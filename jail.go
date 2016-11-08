// +build freebsd, !linux, !darwin

package jail

// #include "jailwrapper.h"
import (
	"C"
)

type Jail struct {
	Wrapper *C.JailWrapper
}

func New(cmd string) *Jail {
	return &Jail{
		Wrapper: C.new_jail_wrapper(C.CString(cmd)),
	}
}

func (j *Jail) Destroy() {
	C.destroy(j.Wrapper)
}
