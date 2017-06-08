// +build freebsd, !linux, !darwin

package jail

// #cgo CFLAGS: -ggdb
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/param.h>
// #include <sys/jail.h>
// #include <sys/types.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #define IN_C false
// #include "jailwrapper.h"
import "C"
import "fmt"

type Jail struct {
	Wrapper *C.JailWrapper
}

func New(cmd string) *Jail {
	cJail := (*C.struct_jail)(C.calloc(6, C.sizeof_jail))
	C.set_path(cJail, C.CString("/tmp/"))
	C.set_hostname(cJail, C.CString("test_jail"))
	C.set_jailname(cJail, C.CString("test_jail"))
	inet := (*C.struct_in_addr)(C.calloc(1, C.sizeof_struct_in_addr))
	C.inet_aton(C.CString("0.0.0.0"), inet)
	C.set_ip4(cJail, inet)
	jid := C.jail_init(cJail)
	fmt.Println(jid)
	pid := C.jail_exec(C.CString(cmd), jid)
	wrapper := (*C.struct_JailWrapper)(C.calloc(3, C.sizeof_JailWrapper))
	C.set_wrapper_pid(wrapper, pid)
	uid := (C.uid_t)(C.getuid())
	C.set_wrapper_uid(wrapper, uid)
	return &Jail{
		Wrapper: wrapper,
	}
}

func (j *Jail) Destroy() {
	C.destroy(j.Wrapper)
}
