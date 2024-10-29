import argparse
import json

H_FILE = """#ifndef QBT_GAIAPROPERTYMANAGER_H
#define QBT_GAIAPROPERTYMANAGER_H

#include <QtCore/QObject>
#include <QtQmlIntegration>
#include "GAIAPropertyBase.h"
#include "GAIAPropertyManagerBase.h"
#include "GAIAPropertyClasses.h"

class GAIAPropertyManager: public GAIAPropertyManagerBase {
    Q_OBJECT

%s

public:
    GAIAPropertyManager(QObject *parent = nullptr);
    GAIAPropertyBase *getPropertyFromVendorCommand(const quint16 &vendorId, const quint16 &commandId) override;

public slots:

%s

signals:

%s

};

#endif //QBT_GAIAPROPERTYMANAGER_H
"""

CPP_FILE = """#include "GAIAPropertyManager.h"

using namespace Qt::Literals::StringLiterals;

GAIAPropertyManager::GAIAPropertyManager(QObject *parent) : GAIAPropertyManagerBase() {}
GAIAPropertyBase *GAIAPropertyManager::getPropertyFromVendorCommand(const quint16 &vendorId, const quint16 &commandId) {
%s
    return nullptr;
}

%s
"""

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Process input and output files.")
    parser.add_argument('-i', '--input', type=str, required=True, help='Input file path')
    parser.add_argument('-o', '--output', type=str, required=True, help='Output file path')

    args = parser.parse_args()
    # print(f"Processing schema {args.input} into {args.output}.{{h,cpp}}")

    with open(args.input, 'r') as f:
        data = json.load(f)

    get_property_code = []
    properties = []
    slots = []
    signals = []
    slot_implementations = []


    with (open(f"{args.output}.h", 'w') as h_file,
          open(f"{args.output}.cpp", 'w') as cpp_file):

        for property_object in data["properties"]:
            get_key = None
            if ("Get" not in property_object
                    and "Set" not in property_object
                    and "Invocation" not in property_object):
                continue
            if "Get" in property_object and "Command" not in property_object["Get"]:
                continue

            if "Get" in property_object:

                vendor_ID = property_object["Get"]["Response"]["gaiaV3"]["vendor_ID"]
                GAIA_command_ID = property_object["Get"]["Response"]["gaiaV3"]["GAIA_command_ID"]

                get_property_code.append("""    if (vendorId == %s && commandId == %s){
            return new %s(this);
        }""" % (vendor_ID, GAIA_command_ID, property_object["name"]))
        # }""" % (property_object["Get"]["Response"]["gaiaV3"]["vendor_ID"], property_object["Get"]["Response"]["gaiaV3"]["GAIA_command_ID"], property_object["name"]))

                properties.append("""    Q_PROPERTY(GAIAPropertyBase* %s READ get%s NOTIFY %sChanged)""" % (property_object["name"], property_object["name"], property_object["name"]))

                slots.append("""    GAIAPropertyBase* get%s();""" % (property_object["name"]))
                signals.append("""    void %sChanged();""" % (property_object["name"]))

                slot_implementations.append(
                    """GAIAPropertyBase *GAIAPropertyManager::get%s() {
    return getProperty("\\x%s\\x%s\\x%s\\x%s"_ba);
}""" % (property_object["name"], vendor_ID[2:4], vendor_ID[4:6], GAIA_command_ID[2:4], GAIA_command_ID[4:6])
                )

                get_key = [vendor_ID, GAIA_command_ID]
            # if "Invocation" in property_object and "Result" in property_object:
            if (property_object.get("Invocation")
                    and property_object.get("Result")
                    and property_object.get("Result", {}).get("Response")
                    and property_object.get("Result", {}).get("Response", {}).get("gaiaV3")):


                vendor_ID = property_object["Result"]["Response"]["gaiaV3"]["vendor_ID"]
                GAIA_command_ID = property_object["Result"]["Response"]["gaiaV3"]["GAIA_command_ID"]
                if get_key:
                    get_property_code.append("""    if (vendorId == %s && commandId == %s){
            return getPropertyFromVendorCommand(%s, %s);
        }""" % (vendor_ID, GAIA_command_ID, get_key[0], get_key[1]))
                else:
                    get_property_code.append("""    if (vendorId == %s && commandId == %s){
            return new %s(this);
        }""" % (vendor_ID, GAIA_command_ID, property_object["name"]))

                properties.append("""    Q_PROPERTY(GAIAPropertyBase* %s READ get%s NOTIFY %sChanged)""" % (property_object["name"], property_object["name"], property_object["name"]))
                slots.append("""    GAIAPropertyBase* get%s();""" % (property_object["name"]))
                signals.append("""    void %sChanged();""" % (property_object["name"]))
                slot_implementations.append(
                    """GAIAPropertyBase *GAIAPropertyManager::get%s() {
    return getProperty("\\x%s\\x%s\\x%s\\x%s"_ba);
}""" % (property_object["name"], vendor_ID[2:4], vendor_ID[4:6], GAIA_command_ID[2:4], GAIA_command_ID[4:6])
                )

        h_file.write(H_FILE % (
            "\n".join(properties),
            "\n".join(slots),
            "\n".join(signals)
        ))
        cpp_file.write(CPP_FILE % (
            "\n".join(get_property_code),
            "\n".join(slot_implementations),
        ))

