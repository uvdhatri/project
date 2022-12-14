//
// SQLCommand.cc
//
// Copyright (c) 2017 Couchbase, Inc All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "CBLiteCommand.hh"
#include "fleece/Expert.hh"
#include "c4Private.h"
using namespace std;
using namespace fleece;


class SQLCommand : public CBLiteCommand {
public:

    SQLCommand(CBLiteTool &parent)
    :CBLiteCommand(parent)
    { }


    void usage() override {
        writeUsageCommand("sql", false, "QUERY");
        cerr <<
        "  Runs a raw SQL query on the database file. This is NOT a way to query your documents!\n"
        "  Rather, it's a very low-level diagnostic tool that will not be useful unless you know the\n"
        "  underlying SQLite schema used by LiteCore.\n"
        ;
    }


    void runSubcommand() override {
        // Read params:
        openDatabaseFromNextArg();
        string sql = restOfInput("sql statement");

        C4Error error;
        alloc_slice fleeceResult = c4db_rawQuery(_db, slice(sql), &error);
        if (!fleeceResult)
            fail("Query failed", error);

        prettyPrint(ValueFromData(fleeceResult), cout);
        cout << '\n';
    }

};


CBLiteCommand* newSQLCommand(CBLiteTool &parent) {
    return new SQLCommand(parent);
}
