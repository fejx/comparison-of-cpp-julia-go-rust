package main

import (
	"fmt"
	"os"
	"time"
)

func main() {
	//runSingle()
	TrySetRealtime()
	fullBenchmark(10)
}

func checkErr(err error) {
	if err == nil {
		return
	}
	_, _ = os.Stderr.WriteString(err.Error())
	os.Exit(1)
}

func runSingle() {
	dimensions := buildDimensions(320, 120)
	image := getImageFor(dimensions)
	generator := NewImageGenerator()
	generator.generate(&image, dimensions)

	err := image.WriteToFile("result.ppm")
	checkErr(err)
}

func fullBenchmark(repetitions int) {
	dimensions := buildDimensions(32000, 12000)
	generator := NewImageGenerator()
	benchmarkDimensions(repetitions, &generator, dimensions)
}

func benchmarkDimensions(repetitions int, generator *ImageGenerator, dimensions ImageDimensions) {
	for i := 0; i < repetitions; i++ {
		image := getImageFor(dimensions)
		duration := performRun(generator, &image, dimensions)
		printRuntime(duration, &image)
	}
}

func performRun(generator *ImageGenerator, image *Image, dimensions ImageDimensions) time.Duration {
	start := time.Now()
	generator.generate(image, dimensions)
	return time.Since(start)
}

func printRuntime(duration time.Duration, image *Image) {
	fmt.Printf("%vx%v;%v\n", image.width, image.height, duration.Seconds())
}

func buildDimensions(width uint, height uint) ImageDimensions {
	return NewImageDimensions(
		width,
		height,
		complex(-2.74529004, 1.23807502),
		complex(1.25470996, -1.01192498),
	)
}

func getImageFor(dimensions ImageDimensions) Image {
	return NewImage(dimensions.width, dimensions.height)
}
