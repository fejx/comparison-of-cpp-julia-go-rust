package main

import "sync"

const maxRectangleSize = 50

type ImageGenerator struct {
	calculator MandelbrotPixelCalculator
}

func NewImageGenerator() ImageGenerator {
	return ImageGenerator{
		NewMandelbrotPixelCalculator(),
	}
}

type ImageGenerationContext struct {
	calculator MandelbrotPixelCalculator
	image      *Image
	dimensions ImageDimensions
	waitGroup  sync.WaitGroup
}

func (generator ImageGenerator) generate(image *Image, dimensions ImageDimensions) {
	assertImageMatchesDimensions(image, dimensions)
	context := ImageGenerationContext{
		generator.calculator,
		image,
		dimensions,
		sync.WaitGroup{},
	}
	fullRectangle := rectangleFromDimensions(dimensions)
	context.workOn(fullRectangle)
	context.waitGroup.Wait()
}

func (context *ImageGenerationContext) workOn(rectangle Rectangle) {
	if context.isTiny(rectangle) {
		context.generateWholeRectangle(rectangle)
	} else {
		context.calculateOutlinesAndSplit(rectangle)
	}
}

func (context *ImageGenerationContext) workOnDone(rectangle Rectangle) {
	context.workOn(rectangle)
	context.waitGroup.Done()
}

func (context *ImageGenerationContext) generateWholeRectangle(rectangle Rectangle) {
	for x := rectangle.x; x < rectangle.endX; x++ {
		for y := rectangle.y; y < rectangle.endY; y++ {
			context.generatePoint(x, y)
		}
	}
}

func (context *ImageGenerationContext) calculateOutlinesAndSplit(rectangle Rectangle) {
	outlinesIdentical, referenceColor := context.generateOutlines(rectangle)

	if outlinesIdentical {
		context.fillWholeRectangle(rectangle, referenceColor)
	} else {
		context.split(rectangle)
	}
}

func (context *ImageGenerationContext) generateOutlines(
	rectangle Rectangle,
) (outlinesIdentical bool, referenceColor Pixel) {
	topY := rectangle.y
	bottomY := rectangle.endY - 1
	leftX := rectangle.x
	rightX := rectangle.endX - 1

	outlinesIdentical = true
	referenceColor = context.generatePoint(leftX, topY)

	for x := leftX; x <= rightX; x++ {
		context.checkOutlinePixel(x, topY, referenceColor, &outlinesIdentical)
		context.checkOutlinePixel(x, bottomY, referenceColor, &outlinesIdentical)
	}
	for y := topY; y <= bottomY; y++ {
		context.checkOutlinePixel(leftX, y, referenceColor, &outlinesIdentical)
		context.checkOutlinePixel(rightX, y, referenceColor, &outlinesIdentical)
	}
	return outlinesIdentical, referenceColor
}

func (context *ImageGenerationContext) checkOutlinePixel(x, y uint, referenceColor Pixel, outlinesIdentical *bool) {
	currentColor := context.generatePoint(x, y)
	if currentColor != referenceColor {
		*outlinesIdentical = false
	}
}

func (context *ImageGenerationContext) split(rectangle Rectangle) {
	parts := SplitRectangle(rectangle)
	for _, part := range parts {
		context.waitGroup.Add(1)
		go context.workOnDone(part)
	}
}

func (context *ImageGenerationContext) fillWholeRectangle(rectangle Rectangle, color Pixel) {
	for x := rectangle.x; x < rectangle.endX; x++ {
		for y := rectangle.y; y < rectangle.endY; y++ {
			target := context.image.Get(x, y)
			*target = color
		}
	}
}

func (context *ImageGenerationContext) generatePoint(x, y uint) Pixel {
	empty := Pixel{}
	target := context.image.Get(x, y)
	if *target != empty {
		return *target
	}
	sampled := context.dimensions.SampleTo(x, y)
	color := context.calculator.GetColorFor(sampled)
	*target = color
	return color
}

func (context *ImageGenerationContext) isTiny(rectangle Rectangle) bool {
	return rectangle.width <= maxRectangleSize && rectangle.height <= maxRectangleSize
}

func assertImageMatchesDimensions(image *Image, dimensions ImageDimensions) {
	if image.height != dimensions.height || image.width != dimensions.width {
		panic("Dimensions do not match")
	}
}

func rectangleFromDimensions(dimensions ImageDimensions) Rectangle {
	return NewRectangle(
		0, 0,
		dimensions.width, dimensions.height,
	)
}
