/* register_types.cpp */

#include "register_types.h"
#include "object_type_db.h"
#include "GDPerlin.h"

void register_GDPerlin_types() {
        ObjectTypeDB::register_type<GDPerlin>();
}

void unregister_GDPerlin_types() {

}
