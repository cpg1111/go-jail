// +build freebsd, !linux, !darwin

package jail

// #include "jailwrapper.h"
import "C"

import (
	"runtime/debug"
	"unsafe"
)

type Jail struct {
	Wrapper unsafe.Pointer
}

func New(cmd string) *Jail {
	var jail *C.JailWrapper
	jail = C.new_jail_wrapper(C.CString(cmd))
	debug.PrintStack()
	return &Jail{
		Wrapper: unsafe.Pointer(jail),
	}
}

func (j *Jail) Destroy() {
	C.destroy(j.Wrapper)
}
