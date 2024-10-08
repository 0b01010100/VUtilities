#ifndef __vstack__
#define __vstack__

#include <stdint.h>
//same as size_t
typedef uint64_t VSTACK_SIZE_T;
//same as ssize_t
typedef int64_t VSTACK_SSIZE_T;
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum VSTACK_VER /* : int*/
{
    VSTACK_VER_0_0 = 0x0,
    VSTACK_VER_1_0 = 0x1
}VSTACK_VER;

/**
 * @brief Enum for specifying which field to query from a vstack.
 */
typedef enum VSTACK_FIELD
{

    VSTACK_FIELD_VER             = 0,  /**< version of the vstack structure */

    //vstack version 0.0

    VSTACK_FIELD_STRIDE          = 1,  /**< Size of each element in the stack */
    VSTACK_FIELD_LENGTH          = 2,  /**< Current number of elements in the stack */
    VSTACK_FIELD_CAPACITY        = 3,  /**< Maximum number of elements the stack can hold */
    VSTACK_FIELD_SCALE_PERCENT   = 4,  /**< Scale factor for dynamic resizing, in percentage */

    //vstack version 1.0

    VSTACK1_FIELD_CTOR           = 5,  /**< Contructor function for each element */
    VSTACK1_FIELD_CCTOR          = 6,  /**< Copy contructor function for each element */
    VSTACK1_FIELD_DTOR           = 7,  /**< Destructor function for each element */
} VSTACK_FIELD;

/**
 * @brief Stack version 0.0 handle
*/
typedef struct vstack vstack;
/**
 * @brief Stack version 1.0 handle
*/
typedef vstack vstack1;

/**
 * @brief Represents a constructor function to be called for each element when the stack is destroyed,
*/
typedef int (*vstack1_el_ctor)(void *location, VSTACK_SIZE_T size, va_list* args, VSTACK_SIZE_T count);

/**
 * @brief Represents a copy constructor function to be called for each element when the stack is destroyed,
*/
typedef int (*vstack1_el_cctor)(void * location, const void * original, VSTACK_SIZE_T size);

/**
 * @brief Represents a destructor function to be called for each element when the stack is destroyed,
*/
typedef void (*vstack1_el_dtor)(void * location, VSTACK_SIZE_T size);

/**
 * @brief Creates a vstack with the specified element stride, initial capacity, and scale factor.
 *
 * @param stride Size of each element in the stack.
 * @param initial_capacity Initial number of elements the stack can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @return Pointer to the newly created vstack, or NULL if creation fails.
 */
vstack* _vstack_create(VSTACK_SIZE_T stride, VSTACK_SIZE_T initial_capacity, double scale_factor);

/**
 * @brief Creates a vstack with the specified element type, initial capacity, and scale factor.
 *
 * @param T Type of an element
 * @param initial_capacity Initial number of elements the stack can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @return Pointer to the newly created vstack, or NULL if creation fails.
 */
#define vstack_create(T, initial_capacity, scale_factor) (vstack*)_vstack_create(sizeof(T), initial_capacity, scale_factor)

/**
 * @brief Creates a vstack with the specified element stride, initial capacity, and scale factor.
 *
 * @param stride Size of each element in the stack.
 * @param initial_capacity Initial number of elements the stack can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @param ctor Constructor function to be called for when a stack is element is created, or NULL if default.
 * @param cctor Copy constructor function to be called for when a stack is element is created, or NULL if default.
 * @param dtor Destructor function to be called for each element when the stack is destroyed, or NULL if default.
 * @return Pointer to the newly created vstack, or NULL if creation fails.
 */
vstack1* _vstack1_create(VSTACK_SIZE_T stride, VSTACK_SIZE_T initial_capacity, double scale_factor, vstack1_el_ctor ctor, vstack1_el_cctor cctor, vstack1_el_dtor dtor);

/**
 * @brief Creates a vstack with the specified element stride, initial capacity, and scale factor.
 *
 * @param T type of an element
 * @param initial_capacity Initial number of elements the stack can hold.
 * @param scale_factor Scale factor for dynamic resizing, in percentage.
 * @param ctor Constructor function to be called for when a stack is element is created, or NULL if default.
 * @param cctor Copy constructor function to be called for when a stack is element is created, or NULL if default.
 * @param dtor Destructor function to be called for each element when the stack is destroyed, or NULL if default.
 * @return Pointer to the newly created vstack, or NULL if creation fails.
 */
#define vstack1_create(T, initial_capacity, scale_factor, ctor, cctor, dtor) (vstack1*)_vstack1_create(sizeof(T), initial_capacity, scale_factor, ctor, cctor, dtor)

/**
 * @brief Retrieves a specific field's value from the vstack.
 *
 * @param sk Pointer to the vstack.
 * @param field Field to retrieve, specified by the VSTACK_FIELD enum.
 * @return Value of the requested field.
 */
VSTACK_SIZE_T vstack_get_field(vstack* sk, VSTACK_FIELD field);

/**
 * @brief Retrieves a specific field's value from the vstack.
 *
 * @param sk Pointer to the vstack.
 * @param field Field to retrieve, specified by the VSTACK_FIELD enum.
 * @param value New value of a field.
 * @note VSTACK_FIELD_STRIDE is not setable.
 * @return Value of the requested field.
 */
void vstack_set_field(vstack* sk, VSTACK_FIELD field, void* value);

/**
 * @brief checks whether the container is empty.
 *
 * @param sk Pointer to the vstack.
 * @return 0 = FALSE, 1 = TRUE, -1 = ERROR
*/
#define vstack_empty(sk) (vstack_get_field(sk, VSTACK_FIELD_LENGTH) == 0X0)

/**
 * @brief Direct access to the underlying contiguous storage
 *
 * @param vec Pointer to the vstack.
 * @return Pointer to the first element in the array
*/
#define vstack_data(vec) vstack_peek(vec)

/**
 * @brief Returns a pointer to the element at the specified index in the vstack.
 *
 * @param sk Pointer to the vstack.
 * @return Pointer to the top element of the stack.
 */
void * vstack_peek(vstack* sk);

/**
 * @brief Copies a object onto the stack.
 *
 * @param sk Pointer to the vstack.
 * @param item Pointer to the item to push onto the stack.
 * @return 0 on success, or -1 on failure.
 */
int vstack_push(vstack* sk, const void* item);

/**
 * @brief Constructs a new item in place at the top of the stack.
 *
 * @param sk Pointer to the vstack.
 * @param count Count of arguments to pass in
 * @param va Arguments to pass in
 * @return 0 on success, or -1 on failure.
 */
int vstack_emplace(vstack* sk, VSTACK_SIZE_T count, ...);

/**
 * @brief Pops the item at the specified index from the vstack.
 *
 * @param sk Pointer to the vstack.
 */
void vstack_pop(vstack* sk);

/**
 * @brief Exchanges the contents and capacity of the container with those of `rhs`.
 * Does not invoke any move, copy, or swap operations on individual elements.
 * @param lhs Pointer ONE vstack.
 * @param rhs Pointer ANOTHER vstack.
*/
void vstack_swap(vstack* lhs, vstack* rhs);

/**
 * @brief Removes the elements in the range [`first`,`last`).
 *
 * @param vec Pointer to the vvector.
 * @note Iterator start is the top of the stack
 * @param last Iterator end
*/
void vvector_erase(vstack* vec, VSTACK_SIZE_T last);

/**
 * @brief Erases all elements from the container.
 *
 * @param vec Pointer to the vstack.
*/
void vstack_clear(vstack* sk);

/**
 * @brief Destroys the vstack and frees associated memory.
 *
 * @param sk Pointer to a pointer to the vstack. The pointer will be set to NULL after destruction.
 */
void vstack_destroy(vstack** sk);

/**
 * Macro to iterate over a stack
 * @param T Type of the item to iterate over
 * @param item A variable of type T that will be assigned each element of the stack
 * @param sk The stack to iterate over
 * @param action The action to perform on each iteration(if any) -> Optional
*/
#define vstack_foreach(T, item, sk, action) {\
    VSTACK_SIZE_T __len = vstack_get_field(sk, VSTACK_FIELD_LENGTH);\
    if(__len != 0) {\
        unsigned char* __data = vstack_data(sk);\
        VSTACK_SIZE_T __stride = vstack_get_field(sk, VSTACK_FIELD_STRIDE);\
        for (VSTACK_SIZE_T __i = 0; __i < __len; __i++) {\
            item = *(int *)(__data - __stride * __i);\
            action\
        }\
    }\
}
#ifdef __cplusplus
}
#endif

#endif // __vstack__
