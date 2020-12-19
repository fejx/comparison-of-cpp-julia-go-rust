push!(LOAD_PATH, pwd())

using Statistics

using BenchmarkUtilities
using ImageM
using KernelM
using Convolution

function performrun(input::Image, output::Image, kernel::Kernel)::Number
	@elapsed convolve!(input, output, kernel)
end

function printruntime(duration::Number, image::Image, kernel::Kernel)
	println("$(kernel.radius);$(image.width)x$(image.height);$duration")
	nothing
end

function benchmarkkernel(
	repetitions::Int, input::Image, output::Image, kernel::Kernel
)
	for i = 1:repetitions
		duration = performrun(input, output, kernel)
		printruntime(duration, input, kernel)
	end
	nothing
end

function benchmarkblur(
	repetitions::Int, radius::Int, input::Image, output::Image
)
	blur = blurkernel(radius)
	benchmarkkernel(repetitions, input, output, blur)
	nothing
end

function fullbenchmark(repetitions::Int, image::Image)
	output = newwithequalsize(image)

	benchmarkblur(repetitions, 10, image, output)
	nothing
end

function runsingle()
	image = readimage("test.ppm")
	output = newwithequalsize(image)
	kernel = blurkernel(2)
	@time convolve!(image, output, kernel)
	writeimage("result.ppm", output)
	println("Written")
	nothing
end

function assertthread()
	threadcount = Threads.nthreads()
	if (threadcount == 1)
		println(
			stderr,
			"WARNING: Running single threaded. " *
			"Set JULIA_NUM_THREADS to enable multithreading."
		)
	end
	nothing
end

assertthread()
trysetrealtime()

# runsingle()
fullbenchmark(10, readimage("sun.ppm"))

function sum(a::Number, b::Number)::Number
	a + b
end