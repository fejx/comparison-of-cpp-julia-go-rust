package main

type ImageDimensions struct {
	width       uint
	height      uint
	topLeft     complex64
	bottomRight complex64
	scaleX      float32
	scaleY      float32
}

func NewImageDimensions(width uint, height uint, topLeft complex64, bottomRight complex64) ImageDimensions {
	scaleX := (real(bottomRight) - real(topLeft)) / float32(width)
	scaleY := (imag(bottomRight) - imag(topLeft)) / float32(height)
	return ImageDimensions{
		width, height,
		topLeft, bottomRight,
		scaleX, scaleY,
	}
}

func (imageDimensions ImageDimensions) SampleTo(x, y uint) complex64 {
	scaledX := float32(x)*imageDimensions.scaleX + real(imageDimensions.topLeft)
	scaledY := float32(y)*imageDimensions.scaleY - imag(imageDimensions.bottomRight)
	return complex(scaledX, scaledY)
}

func (imageDimensions ImageDimensions) SampleToIndex(index uint) complex64 {
	return imageDimensions.SampleTo(
		index%imageDimensions.width,
		index/imageDimensions.width,
	)
}
