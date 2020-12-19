pub mod wait_group {
	use std::sync::{Condvar, Mutex};
	use std::sync::atomic::AtomicUsize;

	use async_std::sync::Arc;

	pub struct WaitGroup {
		wrapped: Arc<InnerWaitGroup>,
	}

	struct InnerWaitGroup {
		conditional: Condvar,
		workers: Mutex<usize>,
	}

	impl WaitGroup {
		pub fn new() -> Self {
			WaitGroup {
				wrapped: Arc::new(InnerWaitGroup {
					conditional: Condvar::new(),
					workers: Mutex::new(1),
				}),
			}
		}

		pub fn wait(self) {
			let wrapped = self.wrapped.clone();
			drop(self);

			let mut workers = wrapped.workers.lock().unwrap();
			while *workers > 0 {
				workers = wrapped.conditional.wait(workers).unwrap();
			}
		}
	}

	impl Clone for WaitGroup {
		fn clone(&self) -> Self {
			let mut workers = self.wrapped.workers.lock().unwrap();
			*workers += 1;
			WaitGroup {
				wrapped: self.wrapped.clone()
			}
		}
	}

	impl Drop for WaitGroup {
		fn drop(&mut self) {
			let mut workers = self.wrapped.workers.lock().unwrap();
			*workers -= 1;

			if *workers == 0 {
				self.wrapped.conditional.notify_all();
			}
		}
	}
}