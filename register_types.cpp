/* register_types.cpp */

#include "register_types.h"
#include "object_type_db.h"
#include "gdperlin.h"

void register_gdperlin_types() {
        ObjectTypeDB::register_type<gdperlin>();
}

void unregister_gdperlin_types() {

}
