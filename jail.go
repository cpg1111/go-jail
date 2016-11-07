package jail

// #include "jailwrapper.h"
import (
	"C"
)

type Jail struct {
	Wrapper C.jailwrapper
}

func New(cmd string) *Jail {
	return &Jail{
		Wrapper: C.new_jail_wrapper(C.CString(cmd)),
	}
}

func (j *Jail) Destroy() {
	j.Wrapper.destroy()
}
