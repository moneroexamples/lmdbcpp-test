

#include "ext/lmdb++.h"

#include <iostream>

using namespace std;

int main() {

    /* Create and open the LMDB environment: */
    auto env = lmdb::env::create();
    env.set_mapsize(1UL * 1024UL * 1024UL * 1024UL); /* 1 GiB */
    env.open("/tmp", MDB_CREATE, 0664);

    /* Insert some key/value pairs in a write transaction: */
    auto wtxn = lmdb::txn::begin(env);
    auto dbi = lmdb::dbi::open(wtxn, nullptr);
    dbi.put(wtxn, "username", "jhacker");
    dbi.put(wtxn, "email", "jhacker@example.org");
    dbi.put(wtxn, "fullname", "J. Random Hacker");
    wtxn.commit();

    /* Fetch key/value pairs in a read-only transaction: */
    auto rtxn = lmdb::txn::begin(env, nullptr, MDB_RDONLY);
    auto cursor = lmdb::cursor::open(rtxn, dbi);
    std::string key, value;
    while (cursor.get(key, value, MDB_NEXT)) {
        std::printf("key: '%s', value: '%s'\n", key.c_str(), value.c_str());
    }
    cursor.close();
    rtxn.abort();

    lmdb::val key1 {"username"};
    lmdb::val  data2;
    string  data3;


    rtxn = lmdb::txn::begin(env, nullptr, MDB_RDONLY);
    dbi.get(rtxn, key1, data2);
    rtxn.abort();




    cout << "Found key: " << string(data2.data(), data2.size()) << endl;


    cout << "Hello, World!" << endl;
    return 0;
}