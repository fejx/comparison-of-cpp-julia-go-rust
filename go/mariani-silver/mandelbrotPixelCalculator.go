package main

import (
	"math"
)

const maximumIterations = 255

type MandelbrotPixelCalculator struct {
	lookupTable [maximumIterations]Pixel
}

func NewMandelbrotPixelCalculator() MandelbrotPixelCalculator {
	hueScale := float32(360) / maximumIterations
	generator := MandelbrotPixelCalculator{}
	lookupTable := &generator.lookupTable
	for i := range lookupTable {
		scaledValue := int(float32(i) * hueScale)
		lookupTable[i] = HSVtoRGB(scaledValue, 1, 1)
	}
	lookupTable[maximumIterations-1] = Pixel{}
	return generator
}

func (generator MandelbrotPixelCalculator) GetColorFor(c complex64) Pixel {
	iterationsWhenDiverging := getIterationsWhenDiverging(c)
	return generator.lookupTable[iterationsWhenDiverging-1]
}

func getIterationsWhenDiverging(seedNumber complex64) uint {
	lastValue := complex64(complex(0, 0))
	i := uint(1)
	for i < maximumIterations {
		lastValue = lastValue * lastValue
		lastValue = lastValue + seedNumber
		if willDiverge(lastValue) {
			break
		}
		i++
	}
	return i
}

func willDiverge(value complex64) bool {
	return absoluteSquared(value) > 4
}

func absoluteSquared(value complex64) float32 {
	realPart := real(value)
	imagPart := imag(value)
	return realPart*realPart + imagPart*imagPart
}

func HSVtoRGB(hue int, saturation float64, value float64) Pixel {
	C := saturation * value
	X := C * (1 - math.Abs(math.Mod(float64(hue)/60, 2)-1))
	m := value - C
	var Rs, Gs, Bs float64

	if hue >= 0 && hue < 60 {
		Rs = C
		Gs = X
		Bs = 0
	} else if hue >= 60 && hue < 120 {
		Rs = X
		Gs = C
		Bs = 0
	} else if hue >= 120 && hue < 180 {
		Rs = 0
		Gs = C
		Bs = X
	} else if hue >= 180 && hue < 240 {
		Rs = 0
		Gs = X
		Bs = C
	} else if hue >= 240 && hue < 300 {
		Rs = X
		Gs = 0
		Bs = C
	} else {
		Rs = C
		Gs = 0
		Bs = X
	}
	red := (Rs + m) * 255
	green := (Gs + m) * 255
	blue := (Bs + m) * 255
	return Pixel{Color(red), Color(green), Color(blue)}
}
