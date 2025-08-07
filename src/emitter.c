// #include "emitter.h"

// int nb_function = 0;

// void put_env_print(FILE *file) {
//   char typeSection[] = {
//       0x01, // Section code: types
//       0x00, // size in bytes
//       0x01, // Number of type defined
//   };

//   char printType[] = {
//       // First type
//       0x60, // type is a function
//       0x01, // number of params
//       0x7d, // first params is f32
//       0x00, // Number of results
//   };

//   typeSection[1] = 5;

//   char importSection[] = {
//       0x02, // Section code: import
//       0x00, // size in bytes
//       0x01, // Number of imports
//   };

//   char printImport[] = {
//       // path of import
//       0x3,  // "env" is 3 bytes
//       0x65, // e
//       0x6e, // n
//       0x76, // v
//       0x05, // "print" is 5 bytes
//       0x70, // p
//       0x72, // r
//       0x69, // i
//       0x6e, // n
//       0x74, // t
//       0x00, // import kind
//       0x00, // function type
//   };

//   importSection[1] = 13;

//   fwrite(typeSection, sizeof(char), 3, file);
//   fwrite(printType, sizeof(char), 4, file);
//   fwrite(importSection, sizeof(char), 3, file);
//   fwrite(printImport, sizeof(char), 12, file);

//   nb_function++;
// }

// // void put_main_function(FILE *file) {
// //   char code[] = {0x03, 0x02, 0x01, 0x00, 0x07, 0x07, 0x01, 0x03, 0x72,
// //                  0x75, 0x6e, 0x00, 0x01, 0x0a, 0x0b, 0x01, 0x09, 0x00,
// //                  0x43, 0x00, 0x00, 0x80, 0x40, 0x10, 0x00, 0x0b};
// //   fwrite(code, sizeof(char), 26, file);
// // }

// void emit(FILE *file) {
//   put_head(file);
//   put_print(file);
//   put_main_function(file);
// }