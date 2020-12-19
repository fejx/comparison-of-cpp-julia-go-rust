package main

import (
	"runtime"
	"sync"
)

func Convolve(input *Image, output *Image, kernel *Kernel) {
	workerCount := runtime.NumCPU()
	var waitGroup sync.WaitGroup
	waitGroup.Add(workerCount)
	for i := 0; i < workerCount; i++ {
		go func(i int) {
			convolveWorker(input, output, kernel, i, workerCount)
			waitGroup.Done()
		}(i)
	}
	waitGroup.Wait()
}

func convolveWorker(
	input *Image, output *Image, kernel *Kernel, xOffset int, xSkip int,
) {
	fallback := Pixel{0, 0, 0}
	radius := kernel.radius
	current := [3]float32{}
	maxColor := int(input.maximumColorValue)
	for x := xOffset; x < input.width; x += xSkip {
		for y := 0; y < input.height; y++ {
			for c := range current {
				current[c] = 0
			}

			for ox := -radius; ox <= radius; ox++ {
				for oy := -radius; oy <= radius; oy++ {
					kernelValue := kernel.Get(ox+radius, oy+radius)
					imageValue := input.GetOr(ox+x, oy+y, &fallback)
					for c := range current {
						current[c] += float32(imageValue[c]) * kernelValue
					}
				}
			}

			pixel := output.Get(x, y)
			for c := range current {
				raw := current[c]
				rounded := Round(raw)
				clamped := Clamp(rounded, 0, maxColor)
				pixel[c] = Color(clamped)
			}
		}
	}
}

func Clamp(n int, lower int, upper int) int {
	if n > upper {
		return upper
	} else if n < lower {
		return lower
	} else {
		return n
	}
}

func Round(n float32) int {
	return int(n + 0.5)
}

func NewImageWithEqualSize(image *Image) Image {
	return NewImage(image.width, image.height, image.maximumColorValue)
}
