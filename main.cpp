

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
    auto dbi = lmdb::dbi::open(wtxn, nullptr, MDB_DUPSORT);
    dbi.put(wtxn, "username", "jhacker");
    dbi.put(wtxn, "email", "jhacker@example.org");
    dbi.put(wtxn, "email", "mwo@example.org");
    dbi.put(wtxn, "fullname", "Middle name");
    dbi.put(wtxn, "job", "accountant");
    dbi.put(wtxn, "email", "dddd@example.org");
    dbi.put(wtxn, "fullname", "J. Random Hacker");
    dbi.put(wtxn, "job", "programmer");
    dbi.put(wtxn, "fullname", "Test Name");
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

    //
    // now put some uint64_t value there
    //

    wtxn = lmdb::txn::begin(env);
    dbi = lmdb::dbi::open(wtxn, nullptr, MDB_DUPSORT);



    uint64_t amount {9382};

    cout << "sizeof(amount): " << sizeof(amount) << endl;

    lmdb::val key_val    {"amount"};
    lmdb::val amount_val {static_cast<void*>(&amount), sizeof(amount)};

    dbi.put(wtxn, key_val, amount_val);


    wtxn.commit();

    //
    // get the amount
    //

    lmdb::val amount_data;

    rtxn = lmdb::txn::begin(env, nullptr, MDB_RDONLY);
    dbi.get(rtxn, key_val, amount_data);
    rtxn.abort();


    cout << "\nAmount found: " << *(amount_data.data<uint64_t>()) << endl;

    cout << endl;



    //
    // get single email
    //


    lmdb::val key1 {"email"};
    lmdb::val  data2;
    string  data3;


    //
    // get a single value
    //

    rtxn = lmdb::txn::begin(env, nullptr, MDB_RDONLY);
    dbi.get(rtxn, key1, data2);
    rtxn.abort();


    cout << "Found key: " << string(data2.data(), data2.size()) << endl;

    cout << endl;

    //
    // retrieve multiple items for same key
    //

    rtxn = lmdb::txn::begin(env, nullptr, MDB_RDONLY);
    cursor = lmdb::cursor::open(rtxn, dbi);

    lmdb::val key2 {"fullname"}, value2;

    // set cursor the the first item
    cursor.get(key2, value2, MDB_SET);

    // process the first item
    cout << "key2: " << string(key2.data(), key2.size())
         << ", value2: " << string(value2.data(), value2.size()) << endl;

    // process other values for the same key
    while (cursor.get(key2, value2, MDB_NEXT_DUP)) {
        cout << "key2: " << string(key2.data(), key2.size())
             << ", value2: " << string(value2.data(), value2.size()) << endl;
    }

    cursor.close();
    rtxn.abort();




    cout << endl;

    cout << "Hello, World!" << endl;
    return 0;
}