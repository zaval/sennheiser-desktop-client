function getDB(){
    let db = LocalStorage.openDatabaseSync("data", "1.0", "", 1000000, (db) => {
        if (db.version === "") {
            db.changeVersion("", "1.0", (tx) => {
                tx.executeSql('CREATE TABLE IF NOT EXISTS devices(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, address TEXT, uuid TEXT)');
            }, (error) => {
                console.log("Error changing version: " + error);
            }, () => {
                console.log("Database version changed successfully");
            });
        }
    });
    return db;
}

function execute(query, callback = undefined){
    const db = getDB();
    db.transaction((tx) => {
        let result = tx.executeSql(query);
        if (callback !== undefined){
            callback(result);
        }

    });
}

function executeWithParams(query, params, callback = undefined){
    const db = getDB();
    db.transaction((tx) => {
        let result = tx.executeSql(query, params);
        if (callback !== undefined){
            callback(result);
        }
    });
}

