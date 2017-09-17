/**
   @file      dbus_adaptor.c
   @author    Frederick Ollinger <https://www.fredollinger.com/>
   @Copyright Copyright 2017 by Frederick Ollinger.  All rights reserved.
   @brief     Simple DBUS client @EOL
   @Keywords  hello world dbus 

*/


#include <stdbool.h>
// #include <string.h>
#include <stdio.h>
//#include <stdlib.h>
#include <unistd.h>
#include <dbus/dbus.h>

#include "dbus_common.h"
#include "dbus_adaptor.h"

DBusConnection *conn;
DBusError err;
int rv;

// BOILERPLATE NOT USED
// We have to satisfy some requirements of libdbus-1, but we don't really need this otherwise
DBusHandlerResult server_message_handler(DBusConnection *conn, DBusMessage *message, void *data)
{
}

// BOILERPLATE NOT USED
// We have to satisfy some requirements of libdbus-1, but we don't really need this otherwise
const DBusObjectPathVTable server_vtable =
{
    .message_function = server_message_handler
};

int dbus_init()
{
    dbus_error_init(&err);

    /* connect to the daemon bus */
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (!conn)
    {
        fprintf(stderr, "Failed to get a session DBus connection: %s\n", err.message);
        goto fail;
    }

    rv = dbus_bus_request_name(conn, HELLO_DBUS_ADAPTOR_INTERFACE_PATH, DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
    if (rv != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)
    {
        printf("Failed to request name on bus: %s\n", err.message);
        goto fail;
    }

    if (!dbus_connection_register_object_path(conn, HELLO_DBUS_ADAPTOR_OBJECT_PATH, &server_vtable, NULL))
    {
        printf("Failed to register a object path\n");
        goto fail;
    }

    return 1;

fail:
    return 0;
}
