#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#if ! defined(__clang__) && ! defined(__GNUC__)
#error clang or gnuc are required
#endif

/*
 DBJ_MX(int,3,4);  makes

 struct { 
    const size_t width; 
    const size_t height; 
    int * e_ptr;
    int(*data)[3][4];
} int_matrix = { 3, 4, (int*)0, calloc(1, sizeof(int[3][4])) };

types are available

DBJ_MX_TYPE(int_matrix) other_mx ;
DBJ_MX_E_TYPE( int_matrix ) pointer_to_mx_value ;

*/
#define DBJ_MX_STRUCT(T,WIDTH,HEIGHT) \
struct { \
    const size_t width; \
    const size_t height; \
    T * e_ptr ; \
    T(*data)[WIDTH][HEIGHT]; \
} 

/*
https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_71/rzarg/typeof_operator.htm
*/
#define DBJ_SWAP(a,b) do { __typeof__(a) temp; temp = a; a = b; b = temp; } while(0)

#define DBJ_MX_TYPE(mx_) __typeof__(mx_) 
// remember this yields type T *, not  T 
#define DBJ_MX_EP_TYPE(mx_) __typeof__(mx_.e_ptr) 
// this is element type
#define DBJ_MX_E_TYPE(mx_) __typeof__(*(mx_.e_ptr)) 

/*
macro callback example : zero the element of the matrix
if flag is set to false, for each stops
    assert( ROW_ < MX_.width ); \
    assert( COL_ < MX_.height); \
*/
#define callback_zoro(ROW_,COL_, MX_, FLAG_ ) \
do { \
    (*MX_.data)[ROW_][COL_] = ( DBJ_MX_E_TYPE(MX_) )0; \
    FLAG_ = true; \
} while(0)

/*
*/
static void dbj_mx_test_one()
{
    enum { W = 3, H = 4 };
    // declare the requied matrix type
    typedef DBJ_MX_STRUCT(int, W, H) int_3_4_mx;

    // define and init the matrix variable
    // NOTE: using VLA in a sizeof 
    // NOTE: can not call calloc/malloc from a global space!
    int_3_4_mx mx = { W, H, (int*)0, calloc(1, sizeof(int[W][H])) };

    ///* set element 1,1 to 0 */
    bool callback_flag;

    callback_zoro(1, 1, mx, callback_flag);

}
/*
*/

#define DBJ_MX_FOREACH_( MX_,  CBACK, GOTOLABEL ) \
do { \
    for (size_t i = 0; i < MX_.width ; i++) \
        for (size_t j = 0; j < MX_.height; j++) { \
            bool retval_ = true; \
            CBACK(i, j, MX_, retval_ ); \
            if (!retval_) goto GOTOLABEL ;\
        } \
    GOTOLABEL : ;\
} while(0)

#define CONCAT_IMPL( x, y ) x##y
#define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )

#define DBJ_MX_FOREACH( MX_,  CBACK ) \
DBJ_MX_FOREACH_( MX_,  CBACK, MACRO_CONCAT( dbj_exit_label_, __COUNTER__) ) \


//static void print(int_3_4_mx mx )
//{
//    const size_t m = mx.width, n = mx.height;
//    for ( size_t i = 0; i < m; i++) {
//        for (size_t j = 0; j < n; j++) {
//            printf("%3d ", * mx.data[i][j]);
//        }
//        printf("\n"); // next row
//    }
//}

// static void multiply(int m, int n, int p, int a[m][n], int b[n][p], int c[m][p])
//static void multiply(int_3_4_mx a, int_4_3_mx b, int_3_3_mx c )
//{
//    const size_t m = a.width, n = a.height, p = b.width;
//    for (int i = 0; i < m; i++) {
//        for (int j = 0; j < p; j++) {
//            * c.data[i][j] = 0;
//            for (int k = 0; k < n; k++) {
//                (* c.data[i][j]) += (* a.data[i][k]) * (* b.data[k][j]);
//            }
//        }
//    }
//}

// make mx instance as compund literal
// us this ad-hoc and localy 
#define DBJ_MX_INIT(T, MXT_,W_,H_) \
(MXT_){ W_, H_, (T*)0, calloc(1, sizeof(T[W_][H_])) }

// make a mx typedef
// also make a factory function creating the instance of
// factory function name is TYPENAME_##_factory
// example: 
// my_matrix_type  my_matrix_type_factory() ;
// returns instances of my_matrix_type

#define DBJ_MX_TYPE_AND_FACTORY(T_, W_, H_, TYPENAME_ ) \
    typedef DBJ_MX_STRUCT(T_, W_, H_) TYPENAME_ ; \
\
    static inline TYPENAME_ TYPENAME_##_factory () { \
        return DBJ_MX_INIT(T_, TYPENAME_,W_,H_) ; \
    }

enum { r1 = 3, c1 = 4, r2 = c1, c2 = r1 };

DBJ_MX_TYPE_AND_FACTORY(int,r1,c1, int34mx );
DBJ_MX_TYPE_AND_FACTORY(int,r2,c2, int43mx );
DBJ_MX_TYPE_AND_FACTORY(int,r1,c2, int33mx );

int mx_struct_test ()
{
    int34mx mx34 = int34mx_factory();
    int43mx mx43 = int43mx_factory();
    int33mx mx33 = int33mx_factory();

    //   int (*mat1)[r1][c1] = malloc( sizeof(int[r1][c1]) ) ;
    //   int (*mat2)[r2][c2] = malloc( sizeof(int[r2][c2]) ) ;
    //   int (*ans )[r1][c2] = malloc( sizeof(int[r1][c2]) ) ;

    DBJ_MX_FOREACH( mx34, callback_zoro );
    DBJ_MX_FOREACH( mx43, callback_zoro );
    DBJ_MX_FOREACH( mx33, callback_zoro );

    return EXIT_SUCCESS;
}