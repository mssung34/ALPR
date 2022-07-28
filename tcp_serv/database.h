#ifndef __DATABASE_H_
#define __DATABASE_H_
#include "/usr/include/mysql/mysql.h"
#include <iostream>
#include <string>
#include <cstring>

class Database
{

public:
    MYSQL *connection = NULL, conn;
    MYSQL_RES *sql_result;
    MYSQL_ROW sql_row;

    Database()
    {
        mysql_init(&conn);
        mysql_options(&conn, MYSQL_SET_CHARSET_NAME, "utf8");
        connection = mysql_real_connect(&conn, "10.10.20.231", "admin", "1q2w3e4r", "alpr", 3306, NULL, 0);
        if (connection == NULL)
        {
            fprintf(stderr, "Failed to connect to databases: Error: %s\n",
                    mysql_error(&conn));
            return;
        }
        else
        {
            //puts("Database connect!");
        }
    }
    ~Database()
    {
        mysql_close(&conn);
        //puts("Database close!");
    }
};
#endif