package main

import (
	"errors"
	"syscall"
)

func trySetRealtime() {
	err := setRealtime()
	checkErr(err)
}

func setRealtime() error {
	const RealtimePriorityClass = 0x100
	current, err := syscall.GetCurrentProcess()
	if err != nil {
		return err
	}
	err = setPriority(current, RealtimePriorityClass)
	if err != nil {
		return err
	}
	return nil
}

func setPriority(handle syscall.Handle, priority int) error {
	dll := syscall.NewLazyDLL("kernel32.dll")
	proc := dll.NewProc("SetPriorityClass")
	r1, _, _ := syscall.Syscall(
		proc.Addr(), 2, uintptr(handle), uintptr(priority), 0,
	)
	if r1 == 0 {
		return errors.New("Error while setting priority")
	}
	return nil
}
