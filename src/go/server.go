package main

/*
#include <world.h>
*/
import "C"

import (
	"fmt"
	"sort"
)

//export Add
func Add(a, b int) int {
	return a + b
}

//export Sub
func Sub(a, b int) int {
	return a - b
}

//export Print
func Print(str string) {
	fmt.Printf("Go prints: %s\n", str)
}

func strFxn(input string) string {
	return "Hello " + input + " World"
}

//export StrFxn
func StrFxn(cinput *C.char) *C.char {
	// C data needs to be manually managed in memory.
	// But we will do it from C++.
	input := C.GoString(cinput)
	return C.CString(strFxn(input))
}

//export Sort
func Sort(vals []int) {
	sort.Ints(vals)
}

func main() {
	// We need the main function to make possible
	// CGO compiler to compile the package as C shared library
	Print(strFxn("Bubba"))
	C.printworld()
}
