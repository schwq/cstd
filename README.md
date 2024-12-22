### [cstd] : Utils for programming in c

*This librarie was made for future projects. It contains a set of code that implements various 
c++ features in c programming, like smart pointers, map, dynamic lists and more*

### Installing (Linux)

*The project uses make for building and installing, clone the entine repository, navigate to it and simply run:*
```bash
make shared
sudo make install
```
*Now you must have a `libcstd.so` and the cstd include folder in your system to use!*

### Usage

- *When building your project, please include `<cstsd/cstd.h>`, it contains everything that you need to include*
- *For compiling and linking, please link the shared librarie: `-lcstd` for gcc*

#### Stack usage:

*In the library, there is a struct that holds a "stack", it is a static variable that holds every important memory allocation 
and information, so it can manage it for you, but remember, you need to initialize it and end it of your project!*
```c
#include <cstd/cstd.h>

int main() {
    initialize_stack(); //-> Initializes the static variable
    // Everything elses goes between!
    end_stack(); // -> Free everything that wasnt freed properly!
    return 0;
}
```

*The stack also supports deallocation by different levels. The c programming language does not implements a way to know 
when the variable is going out of scope, so the library provides two functions to create this piece of scope for example:*

```c
#include <cstd/cstd.h>

void function() {
    push_stack(); // -> Pushs a level to the stack, current level is 1

    // Creates a smart_ptr in the current level (1)
    smart_ptr_t* smart_ptr1 = smart_alloc(sizeof(char) * 3, "ABC");

    // Calls the smart_release() on the smart_ptr1, because the level is going down to 0 again,
    // and kills the smart_ptr
    pop_stack(); 
}

int main() {
    initialize_stack(); // -> Stack start, current level is 0

    function(); // -> The function is called

    smart_ptr_t* smart_ptr2 = smart_alloc(sizeof(char) * 11, "Hello World");

    end_stack(); // -> Stack ends, every other smart_ptr is killed here. smart_ptr2 is gone now
    return 0;
}

```

*The programmer must be aware of calling `push_stack()` and `pop_stack()`, even so, the `end_stack()` will deallocate every smart pointer alive, independent of the level that the pointer holds*

### Smart Pointers

*Smart pointer are wrappers around raw pointers, it holds a reference counter and a level. When passing a smart pointer, the reference increase by 1, to notify the pointer that some part of the code it still using it. When that part is done using the pointer, it calls a release function that decrease the reference counter by 1. If the reference counter goes to 0, the smart pointer is finally free. The level is a number that represents the scope that the smart pointer was created, when the smart pointer goes out of scope, the level of the scope if checked and if the scope and the smart pointer have the same level, the reference counter decrease*

```c
#include <cstd/cstd.h>

static smart_ptr_t *smart_ptr4;

void function() {
  push_stack();
  
  // Allocate smart_ptr3 with data "GIT"
  smart_ptr_t *smart_ptr3 = smart_alloc(sizeof(char) * 3, "GIT");
  // Asign smart_ptr4 to smart_ptr3
  smart_ptr4 = smart_grab(smart_ptr3);
  
  // smart_ptr3 level is equal to the pop_stack() level, it decrease one reference
  // but it wont be free, because smart_ptr4 grab it early
  pop_stack(); 
}

int main() {
  initialize_stack(); // -> Start stack

  // Allocate smart_ptr1 with "Hello world" data, level 0, reference 1
  smart_ptr_t *smart_ptr1 = smart_alloc(sizeof(char) * 11, "Hello world");
  // Creates smart_ptr2 that grabs smart_ptr1, it now have reference 2
  smart_ptr_t *smart_ptr2 = smart_grab(smart_ptr1);

  // Releases the smart_ptr1, but it wont be free, because there is 1 reference left    
  smart_release(smart_ptr1);  
  // Releases smart_ptr2 that points to smart_ptr1, now it have reached 0 references, it is free here!
  smart_release(smart_ptr2);

  function();
  
  // Prints "Smart Pointer 3 data is: GIT", smart_ptr4 points to the smart_ptr3 data!
  printf("Smart Pointer 3 data is: %s\n", (const char *)smart_ptr4->data->pointer);

  // End stack, smart_ptr4 have not been released, so the stack kill it with smart_ptr3
  end_stack(); // No leaks in this program!
  return 0;
}
```

#### Beware releases and pop_stacks!
*You must remember that `pop_stack()` is going to call a release function to all smart pointers created in that level! If you put a `pop_stack()` in a function, and before hitting it calls a release to the smart pointer, it can be free without telling you, for example taking the last code:*

```c 

#include <cstd/cstd.h>

static smart_ptr_t* smart_ptr1;

void function() {
    push_stack();

    // Allocates smart_ptr2 with the data "Hello world". Level 1, reference count 1
    smart_ptr_t* smart_ptr2 = smart_alloc(sizeof(char) * 11, "Hello world");
    // Creates smart_ptr1 with smart_ptr2 data, that have reference counter 2 now
    smart_ptr1 = smart_grab(smart_ptr2);
    smart_release(smart_ptr2); // smart_ptr2 is release, and reference counter is 1

    pop_stack(); // smart_ptr2 is release here too! and free, because the reference counter is 0
}

int main() {
    initialize_stack();

    function(); 
    // Segmentation fault! smart_ptr2 was free inside function()
    printf("Smart pointer 2 data is: %s", (const char*)smart_ptr1->data->pointer);

    end_stack();
    return 0;
}
```
*Best way to avoid this kind of error? Let the program handle itself the pointer sometimes*

*`pop_stack()` is allow to only take one reference away from the smart pointer in their lifetime, there is a variable inside the smart pointer that controls if the pointer have lost a reference by the scope, if so, others `pop_stack()` called during execution wont free the smart pointer and wont cause a segmentation fault*

### Dynamic lists

*Dynamics lists are like array in c, the difference is that the size is change overtime. When we allocate a array in c, we must specify the size in the beginning that wont change! Dynamics lists can have multiple elements inside of it, if the space is filled, it wiil allocate more space*

```c
#include <cstd/cstd.h>

// In this example, we create a struct to hold interger values
typedef struct INTERGER_T {

  int *value;

} interger_t;

// Creates a function that creates the struct and allocate everything needed
interger_t *make_int(int value) {
  interger_t *_int = malloc(sizeof(struct INTERGER_T));
  _int->value = malloc(sizeof(int));
  *_int->value = value;
  return _int;
}

// Creates a function that retrieves a pointer and frees everything
void free_int(interger_t *_int) {
  free(_int->value);
  free(_int);
}

int main() {

  // Creates a list that holds pointers to interger structs!
  dynamic_list_t *list = initialize_list(sizeof(struct INTERGER_T));

  // Appends a struct pointer to the list
  // We can make multiple appends to the list, it wiil grows in size and allocate more memory
  append_list(list, make_int(10));
  
  // Gets the first value of the list, converts it to struct pointer and prints value
  printf("Interger value is %i", *((interger_t *)(list->pointer[0]))->value);

  append_list(list, make_int(90));
  append_list(list, make_int(1000));
  append_list(list, make_int(7));
  
  // Removes the element with index 0, in the given list, and uses the free function provided 
  remove_list(list, 0, free_int);

  // Frees all elements inside the list with the free function passed and frees the list pointer itself
  free_list(list, free_int);

  return 0;
}
```
*The free function can be a function that the programmer created, like this case, or if none is passed, the default `free()` is used*

```c 
  remove_list(list, nullptr); // -> free() is used to deallocate elements! Not the best idea 
```

*Remember that to get the value from a smart pointer or from a dynamic list, you must cast the void\* pointer provide to the exactly struct pointer that the dynamic list or the smart pointer holds, so you can acess it variables*

```c 
  // Get the element from the list with index 0, cast to the struct pointer, get the variable (value, that is a pointer)
  // and get the data pointed by the pointer
  printf("Interger value is %i", *((interger_t *)(list->pointer[0]))->value);
```