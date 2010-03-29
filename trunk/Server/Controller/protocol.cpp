 /* protocol.cpp 
 *
 * Modified on:    $Date$
 * Last Edited By: $Author$
 * Revision:       $Revision$
 */
#include"protocol.h"

 int write_data(int type, void* data_, short number, byteArray* byte_ptr){
    if(!data_ | !byte_ptr)
        return P_NULL_PTR;

    char* array;
    unsigned short* sizes = new unsigned short[number];
    int overall_size = 0;
    char** structs = new char*[number];
    switch(type){

        case P_ROBOT_INIT:
            {
                robotInit* data = (robotInit*)data_;
                for(int i = 0; i < number; ++i){
                    short str_len = data[i].VideoURL->size() + 1;
                    const char* url = data[i].VideoURL->c_str();
                    short size = sizeof(int)*3 + str_len + sizeof(short)*2;
                    structs[i] = new char[size];

                    //put the size on the top of the array
                    char* ref = (char*)&size;
                    for(int j = 0; j < 2; ++j){
                        structs[i][j] = ref[j];
                    }

                    //push rid on the array
                    ref = (char*)&(data[i].RID);
                    for(int j = 2; j < 6; ++j){
                        structs[i][j] = ref[j-2];
                    }

                    //push x
                    ref = (char*)&(data[i].x);
                    for(int j = 6; j <10; ++j){
                        structs[i][j] = ref[j-6];
                    }

                    //push y
                    ref = (char*)&(data[i].y);
                    for(int j = 10; j < 14; ++j){
                        structs[i][j] = ref[j-10];
                    }

                    //push the string size
                    ref = (char*)&str_len;
                    for(int j = 14; j < 16; ++j){
                        structs[i][j] = ref[j-14];
                    }

                    //push the string
                    for(int j = 16; j < 16+str_len; ++j){
                        structs[i][j] = url[j - 16];
                    }

                    //add the length of this array to the total length and store it
                    sizes[i] = size;
                    overall_size += size;
                }
                break;
            }

        case P_ROBOT_UPDATE:
            {
                robotUpdate* data = (robotUpdate*)data_;
                for (int i = 0; i < number; ++i){
                    short size = sizeof(int) * 4 + sizeof(short) + data[i].listSize * 4;
                    structs[i] = new char[size];

                    //push size ontop of the array
                    char* ref = (char*)&size;
                    for(int j = 0; j < 2; ++j){
                        structs[i][j] = ref[j];
                    }
                    
                    //push rid 
                    ref = (char*)&(data[i].RID);
                    for(int j = 2; j < 6; ++j){
                        structs[i][j] = ref[j-2];
                    }

                    //push x
                    ref = (char*)&(data[i].x);
                    for(int j = 6; j <10; ++j){
                        structs[i][j] = ref[j-6];
                    }

                    //push y
                    ref = (char*)&(data[i].y);
                    for(int j = 10; j < 14; ++j){
                        structs[i][j] = ref[j-10];
                    }

                    ref = (char*)&(data[i].listSize);
                    for(int j = 14; j < 18; ++j){
                        structs[i][j] = ref[j - 14];
                    }

                    ref = (char*)(data[i].list);
                    for(int j = 18; j <18 + data[i].listSize * 4; ++i){
                        structs[i][j] = ref[j-18];
                    }

                    sizes[i] = size;
                    overall_size += size;
                }
                break;
            }
            
        case P_OBJECT:
            {
                object* data = (object*)data_;
                for (int i = 0; i < number; ++i) {
                    int str_len = data[i].name->size() + 1;
                    const char* name = data[i].name->c_str();
                    int size = sizeof(int) * 4 + str_len + data[i].color_size + 1;

                    structs[i] = new char[size];

                    int idx = 0;
                    // push size on
                    char* ref = (char*)&size;
                    for (int j = idx; j < (int)(idx + sizeof(int)); ++j) {
                        structs[i][j] = ref[j];
                    }
                    idx += sizeof(int);

                    // push OID
                    ref = (char*)&(data[i].OID);
                    for (int j = idx; j < (int)(idx + sizeof(int)); ++j) {
                        structs[i][j] = ref[j - idx];
                    }
                    idx += sizeof(int);

                    // push strlen
                    ref = (char*)&str_len;
                    for (int j = idx; j < (int)(idx + sizeof(int)); ++j) {
                        structs[i][j] = ref[j - idx];
                    }
                    idx += sizeof(int);

                    // push string
                    for (int j = idx; j < (idx + str_len); ++j) {
                        structs[i][j] = name[j - idx];
                    }
                    idx += str_len;

                    // push color_size
                    ref = (char*)&(data[i].color_size);
                    for (int j = idx; j < (int)(idx + sizeof(int)); ++j) {
                        structs[i][j] = ref[j - idx];
                    }
                    idx += sizeof(int);

                    // push color
                    for (int j = idx; j < (idx + data[i].color_size + 1); ++j) {
                        structs[i][j] = data[i].color[j - idx];
                    }
                    idx += data[i].color_size + 1;

                    // add the length of this array to the total length and store it
                    sizes[i] = size;
                    overall_size += size;
                }
                break;
            }
            case P_ASSIGNMENT:
            {
                assignment* data = (assignment*)data_;  
                short size = 2*sizeof(int) + sizeof(short);
                for(int i = 0; i < number; ++i){
                    structs[i] = new char[size];

                    //push size 
                    char* ref = (char*)(&size);
                    for(int j = 0; j < 2; ++j){
                        structs[i][j] = ref[j];
                    }

                    //push rid 
                    ref = (char*)&(data[i].RID);
                    for(int j = 2; j < 6; ++j){
                        structs[i][j] = ref[j-2];
                    }

                    //push 0ID
                    ref = (char*)&(data[i].OID);
                    for(int j = 6; j <10; ++j){
                        structs[i][j] = ref[j-6];
                    }
                    
                    sizes[i] = size;
                    overall_size += size;
                }

            }
            break;
            default:
                return P_INVD_TYPE;

    }

    //intiallize the array to return
    int length = overall_size + 1 + 2 + 4;
    array = new char[length];

    //set type
    array[0] = type;

    //push length in bytes
    char* ref = (char*)&length;
    for(int i = 1; i < 5; ++i){
        array[i] = ref[i-1];
    }

    //push number of elements;
    ref = (char*)&number;
    for(int i = 5; i < 7; ++i){
        array[i] = ref[i-5];
    }
    
    //push each array onto the array
    int position = 7;
    for(int i = 0; i < number; ++i){
             memcpy(&array[position], structs[i], sizes[i]);
             position += sizes[i];
    }
    //clean up
    delete[] sizes;
    for(int i = 0; i < number; ++i){
        delete[] structs[i];
    }
    delete[] structs;
    byte_ptr->array = array;
    byte_ptr->size = length;
    
    return P_OK;

}

int readRobotInit(void* array, robotInit* &robots) {
    char* arr = (char*)array;
    char* current = arr+5;
    char* ref;

    // overall size
    short overall_size;
    ref = (char*)&overall_size;
    ref[0] = current[0]; current++;
    ref[1] = current[0]; current++;

    robots = new robotInit[overall_size];

    for (int i = 0; i < overall_size; ++i) {
        short size;
        int rid;
        int x;
        int y;
        short str_len;
        char* url;

        
        // retreive size
        ref = (char*)&size;
        ref[0] = current[0]; current++;
        ref[1] = current[0]; current++;

        // retrieve RID
        ref = (char*)&rid;
        for (int j = 0; j < (int)sizeof(int); ++j) {
            ref[j] = current[0]; current++;
        }

        // retrieve x
        ref = (char*)&x;
        for (int j = 0; j < (int)sizeof(int); ++j) {
            ref[j] = current[0]; current++;
        }

        // retrieve y
        ref = (char*)&y;
        for (int j = 0; j < (int)sizeof(int); ++j) {
            ref[j] = current[0]; current++;
        }

        // retrieve str_len
        ref = (char*)&str_len;
        for (int j = 0; j < (int)sizeof(short); ++j) {
            ref[j] = current[0]; current++;
        }

        // retrieve url
        url = new char[str_len];
        for (int j = 0; j < str_len; ++j) {
            url[j] = current[0]; current++;
        }

        // build the robotInit struct
        robots[i].RID = rid;
        robots[i].x = x;
        robots[i].y = y;
        robots[i].VideoURL = new std::string(url);

        delete[] url;
    }
    return overall_size;
}

int readRobotUpdate(void* array, robotUpdate* &robots){

    char* arr = (char*)array;
    char* current = arr+5;
    char* ref;

    // overall size
    short overall_size;
    ref = (char*)&overall_size;
    ref[0] = current[0]; current++;
    ref[1] = current[0]; current++;

    robots = new robotUpdate[overall_size];
    
    for (int i = 0; i < overall_size; ++i) {
        short size;
        int rid;
        int x;
        int y;
        int listSize;
        int* list;
        
        // retreive size
        ref = (char*)&size;
        ref[0] = current[0]; current++;
        ref[1] = current[0]; current++;

        // retrieve RID
        ref = (char*)&rid;
        for (int j = 0; j < (int)sizeof(int); ++j) {
            ref[j] = current[0]; current++;
        }

        // retrieve x
        ref = (char*)&x;
        for (int j = 0; j < (int)sizeof(int); ++j) {
            ref[j] = current[0]; current++;
        }

        // retrieve y
        ref = (char*)&y;
        for (int j = 0; j < (int)sizeof(int); ++j) {
            ref[j] = current[0]; current++;
        }

        //retrieve listSize;
        ref = (char*)&listSize;
        for(int j = 0; j < (int)sizeof(int); ++j){
            ref[j] = current[0]; current++;
        }

        list = new int[listSize];
        ref = (char*)list;
        for(int j = 0; j < listSize*4; ++j){
            ref[j] = current[0]; current++;
        }

        // build the robotInit struct
        robots[i].RID = rid;
        robots[i].x = x;
        robots[i].y = y;
        robots[i].listSize = listSize;
        robots[i].list = list;
    }

    return overall_size;
}

int readObject(void* array, object* &objs) {
    char* arr = (char*)array;
    char* current = arr+5;
    char* ref;

    // overall size
    short overall_size;
    ref = (char*)&overall_size;
    ref[0] = current[0]; current++;
    ref[1] = current[0]; current++;

    objs = new object[overall_size];

    for (int i = 0; i < overall_size; ++i) {
        int size;
        int oid;
        int str_len;
        char* name;
        char* color;
        int color_size;

        
        // retreive size
        ref = (char*)&size;
        ref[0] = current[0]; current++;
        ref[1] = current[0]; current++;
        ref[2] = current[0]; current++;
        ref[3] = current[0]; current++;

        // retrieve OID
        ref = (char*)&oid;
        for (int j = 0; j < (int)sizeof(int); ++j) {
            ref[j] = current[0]; current++;
        }

        // retrieve str_len
        ref = (char*)&str_len;
        for (int j = 0; j < (int)sizeof(int); ++j) {
            ref[j] = current[0]; current++;
        }

        // retrieve name 
        name = new char[str_len];
        for (int j = 0; j < str_len; ++j) {
            name[j] = current[0]; current++;
        }

        // retrieve color_size 
        ref = (char*)&color_size;
        for (int j = 0; j < (int)sizeof(int); ++j) {
            ref[j] = current[0]; current++;
        }

        // retrieve color 
        color = new char[color_size];
        for (int j = 0; j < color_size; ++j) {
            color[j] = current[0]; current++;
        }

        // build the object struct
        objs[i].OID = oid;
        objs[i].name = new std::string(name);
        objs[i].color_size = color_size;
        objs[i].color = color;

        delete[] name;
    }
    return overall_size;
}

int readAssignment(void* array, assignment* &ass){
    
    char* arr = (char*)array;
    char* current = arr+5;
    char* ref;

    // overall size
    short overall_size;
    ref = (char*)&overall_size;
    ref[0] = current[0]; current++;
    ref[1] = current[0]; current++;

    ass = new assignment[overall_size];

    for (int i = 0; i < overall_size; ++i) {
        short size;
        int rid;
        int oid;
        
        // retreive size
        ref = (char*)&size;
        ref[0] = current[0]; current++;
        ref[1] = current[0]; current++;

        // retrieve RID
        ref = (char*)&rid;
        for (int j = 0; j < (int)sizeof(int); ++j) {
            ref[j] = current[0]; current++;
        }

        // retrieve x
        ref = (char*)&oid;
        for (int j = 0; j < (int)sizeof(int); ++j) {
            ref[j] = current[0]; current++;
        }

        //initallize struct
        ass[i].RID = rid;
        ass[i].OID = oid;
    }
    return overall_size;
}

// takes a void pointer to the data as output from write_data and a readReturn struct to put the array and its type in.
int read_data(void* array, readReturn* ret){
    
    if(!ret || !array)
        return P_NULL_PTR;
    
    switch( (char) ((char*)array)[0] ) {
        case P_ROBOT_INIT:
            {
                robotInit* arr;
                int count = readRobotInit(array, arr);

                // Create a readReturn if we have a NULL pointer
                //if (!ret)
                //    ret = new readReturn
                ret->array = (void*)arr;
                ret->size = count;
                ret->type = P_ROBOT_INIT;

                return count;
            }

        case P_ROBOT_UPDATE:
            {
                robotUpdate* arr;
                int count =readRobotUpdate(array, arr);

                ret->array = (void*)arr;
                ret->size = count;
                ret->type = P_ROBOT_UPDATE;

                return count;
            }
            break;

        case P_OBJECT:
            {
                object* arr;
                int count =readObject(array, arr);

                ret->array = (void*)arr;
                ret->size = count;
                ret->type = P_OBJECT;

                return count;
            }
            break;
        
        case P_ASSIGNMENT:
            {
                assignment* arr;
                int count =  readAssignment(array, arr);

                ret->array = (void*)arr;
                ret->size = count;
                ret->type = P_ASSIGNMENT;
                
                return count;
            }

        default:

            std::cerr << "Attempt to read unknown type\n";
    }
    return P_INVD_TYPE;
}

/* vi: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */