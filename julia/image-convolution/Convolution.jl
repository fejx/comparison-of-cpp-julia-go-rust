module Convolution

using ImageM
using KernelM

export convolve!, newwithequalsize

function convolve!(input::Image, output::Image, kernel::Kernel)
	default = zeros(Color, 3)
	radius = kernel.radius
	maxColor = input.maximumColorValue
	Threads.@threads for x in 1:input.width
		for y in 1:input.height
			current = zeros(Float16, 3)

			for ox in -radius:radius, oy in -radius:radius
				kernelValue = kernel.matrix[oy + radius + 1, ox + radius + 1]
				imageValues = getator(input, ox + x, oy + y, default)
				current += imageValues * kernelValue
			end

			output.pixels[y, x, :] = clamp.(round.(current), 0, maxColor)
		end
	end
	nothing
end

function newwithequalsize(image::Image)::Image
	Image(image.width, image.height, image.maximumColorValue)
end

function getator(image::Image, x::Int, y::Int, default::Array{Color,1})::Array{Color, 1}
	xrange = 1:image.width
	yrange = 1:image.height
	if isin(xrange, x) && isin(yrange, y)
		@view image.pixels[y, x, :]
	else
		default
	end
end

function isin(range::UnitRange{T}, value::T) where {T}
	range.start <= value && value <= range.stop
end

end