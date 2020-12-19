module KernelM

export Kernel, blurkernel

struct Kernel
	radius::Int
	width::Int
	totalSize::Int
	matrix::Array{Float32,2}
end

function Kernel(radius::Int)::Kernel
	width = radius * 2 + 1
	totalSize = sq(width)
	matrix = zeros(Float32, width, width)
	Kernel(radius, width, totalSize, matrix)
end

function blurkernel(radius::Int)::Kernel
	k = Kernel(radius)
	value = inv(k.totalSize)
	k.matrix .= value
	k
end

sq(x) = x * x

end
