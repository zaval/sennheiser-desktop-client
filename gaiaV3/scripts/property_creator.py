import argparse
import json
import pathlib
import re

H_HEADER = """#ifndef QBT_GAIAPROPERTYCLASSES_H
#define QBT_GAIAPROPERTYCLASSES_H

#include "GAIAPropertyBase.h"
#include <QObject>
#include <QtQmlIntegration>

"""

H_FOOTER = """
#endif
"""

CPP_HEADEAR = """#include "GAIAPropertyClasses.h"
"""

value_types = {
    "UINT8": "ParameterType::UINT8",
    "UINT16": "ParameterType::UINT16",
    "UINT32": "ParameterType::UINT32",
    "STRING": "ParameterType::STRING",
}

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Process input and output files.")
    parser.add_argument('-i', '--input', type=str, required=True, help='Input file path')
    parser.add_argument('-o', '--output', type=str, required=True, help='Output file path')

    args = parser.parse_args()

    with open(args.input, 'r') as f:
        data = json.load(f)

    with (open(f"{args.output}.h", 'w') as h_file,
          open(f"{args.output}.cpp", 'w') as cpp_file):

        h_file.write(H_HEADER)
        cpp_file.write(CPP_HEADEAR)

        for property_object in data["properties"]:

            if ("Get" not in property_object
                    and "Set" not in property_object
                    and "Invocation" not in property_object):
                continue
            if "Get" in property_object and "Command" not in property_object["Get"]:
                continue

            h_file.write("""
class %s: public GAIAPropertyBase {
    Q_OBJECT
    QML_ELEMENT

public:
    explicit %s(QObject *parent = nullptr);
};
""" % (property_object["name"], property_object["name"]))

            cpp_file.write("""
%s::%s(QObject *parent) : GAIAPropertyBase(parent) {
""" % (property_object["name"], property_object["name"]))

            is_valueTypes_set = False

            if "Get" in property_object:
                cpp_file.write("""    getVendorId = %s;
""" % (property_object["Get"]["Command"]["gaiaV3"]["vendor_ID"]))
                cpp_file.write("""    getCommandId = %s;
""" % (property_object["Get"]["Command"]["gaiaV3"]["GAIA_command_ID"]))
                cpp_file.write("""    getResponseVendorId = %s;
""" % (property_object["Get"]["Response"]["gaiaV3"]["vendor_ID"]))
                cpp_file.write("""    getResponseCommandId = %s;
""" % (property_object["Get"]["Response"]["gaiaV3"]["GAIA_command_ID"]))
                cpp_file.write("""    valueTypes = {%s};
""" % ", ".join([f"ParameterType::{x['type']}" for x in property_object["Get"]["Response"]["parameter"]]))
                is_valueTypes_set = True

            if ("Set" in property_object and
                    "Command" in property_object["Set"] and
                    isinstance(property_object["Set"]["Command"], dict)):
                cpp_file.write("""    setVendorId = %s;
""" % (property_object["Set"]["Command"]["gaiaV3"]["vendor_ID"]))
                cpp_file.write("""    setCommandId = %s;
""" % (property_object["Set"]["Command"]["gaiaV3"]["GAIA_command_ID"]))
                cpp_file.write("""    setResponseVendorId = %s;
""" % (property_object["Set"]["Response"]["gaiaV3"]["vendor_ID"]))
                cpp_file.write("""    setResponseCommandId = %s;
""" % (property_object["Set"]["Response"]["gaiaV3"]["GAIA_command_ID"]))
                if not is_valueTypes_set:
                    cpp_file.write("""    valueTypes = {%s};
""" % ", ".join([f"ParameterType::{x['type']}" for x in property_object["Set"]["Command"]["parameter"]]))

                cpp_file.write("""    setTypes = {%s};
""" % ", ".join([f"ParameterType::{x['type']}" for x in property_object["Get"]["Response"]["parameter"]]))

            if ("Invocation" in property_object and
                    "Command" in property_object["Invocation"] and
                    isinstance(property_object["Invocation"]["Command"], dict) and
            "gaiaV3" in property_object["Invocation"]["Command"] ):
                cpp_file.write("""    invocationVendorId = %s;
""" % (property_object["Invocation"]["Command"]["gaiaV3"]["vendor_ID"]))
                cpp_file.write("""    invocationCommandId = %s;
""" % (property_object["Invocation"]["Command"]["gaiaV3"]["GAIA_command_ID"]))
                cpp_file.write("""    invocationResponseVendorId = %s;
""" % (property_object["Result"]["Response"]["gaiaV3"]["vendor_ID"]))
                cpp_file.write("""    invocationResponseCommandId = %s;
""" % (property_object["Result"]["Response"]["gaiaV3"]["GAIA_command_ID"]))
                cpp_file.write("""    resultTypes = {%s};
""" % ", ".join([f"ParameterType::{x['type']}" for x in property_object["Result"]["Response"].get("parameter", [])]))
                if "parameter" in property_object["Invocation"]["Command"]:
                    cpp_file.write("""    parameterTypes = {%s};
""" % ", ".join([f"ParameterType::{x['type']}" for x in property_object["Invocation"]["Command"]["parameter"]]))
            cpp_file.write("}\n")


        h_file.write(H_FOOTER)
