package jail

import "testing"

func TestNew(t *testing.T) {
	jail := New("ping google.com")
	t.Logf("%+v\n", jail)
}
