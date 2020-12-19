module BenchmarkUtilities

export trysetrealtime

const REALTIME_PRIORITY_CLASS = 0x100

function trysetrealtime()
	current = getcurrent()
	setpriority(current, REALTIME_PRIORITY_CLASS)
end

function getcurrent()::Ptr
	ccall(:GetCurrentProcess, Ptr{Cvoid}, ())
end

function setpriority(handle::Ptr, priority)
	result = ccall(:SetPriorityClass, Cuchar, (Ptr{Cvoid}, Culong), handle, priority)
	if (result == 0)
		error("Error while setting priority")
	end
end

end