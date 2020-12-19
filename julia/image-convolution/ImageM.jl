module ImageM

export Color, Image, readimage, writeimage

const Color = UInt8

struct Image
	width::Int
	height::Int
	maximumColorValue::Color
	pixels::Array{Color,3}
end

function Image(width::Int, height::Int, maximumColorValue::Color)
	pixels = zeros(UInt, height, width, 3)
	Image(width, height, maximumColorValue, pixels)
end

function readimage(filename::AbstractString)::Image
	open(filename, "r") do io
		header = readline(io)
		if (header != "P6")
			error("Not a ppm file")
		end
		width = readuntil(io, " ")
		height = readline(io)
		maximumColorValue = readline(io)
		width = parse(Int, width)
		height = parse(Int, height)
		maximumColorValue = parse(Color, maximumColorValue)
		if (maximumColorValue > 255)
			error("Maximum color values above 255 are not supported")
		end
		image = Image(width, height, maximumColorValue)
		readpixels!(image.pixels, io)
		return image
	end
end

function writeimage(filename::AbstractString, image::Image)
	open(filename, "w") do io
		println(io, "P6")
		println(io, "$(image.width) $(image.height)")
		println(io, image.maximumColorValue)
		writepixels(io, image.pixels)
	end
end

function readpixels!(pixels::Array{Color,3}, io::IO)
	for y in 1:size(pixels, 1), x in 1:size(pixels, 2), c in 1:size(pixels, 3)
		v = read(io, 1)
		pixels[y, x, c] = v[1]
	end
end

function writepixels(io::IO, pixels::Array{Color,3})
	for y in 1:size(pixels, 1), x in 1:size(pixels, 2), c in 1:size(pixels, 3)
		v = UInt8(pixels[y, x, c])
		write(io, v)
	end
end

end
