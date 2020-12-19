extern crate libc;

pub mod benchmark_utilities {
	use std::os::raw::c_void;

	type Handle = *mut c_void;
	type Dword = libc::c_uint;

	const REALTIME_PRIORITY_CLASS: Dword = 0x100;

	pub fn set_realtime() {
		unsafe {
			let current = GetCurrentProcess();
			SetPriorityClass(current, REALTIME_PRIORITY_CLASS);
		}
	}

	#[link(name = "kernel32")]
	extern {
		fn GetCurrentProcess() -> Handle;
		fn SetPriorityClass(handle: Handle, priority: Dword) -> bool;
	}
}