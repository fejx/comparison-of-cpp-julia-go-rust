package main

import (
	"fmt"
	"os"
	"time"
)

func main() {
	trySetRealtime()
	// runSingle()
	fullBenchmark(10, "bridge.ppm")
}

func checkErr(err error) {
	if err == nil {
		return
	}
	_, _ = os.Stderr.WriteString(err.Error())
	os.Exit(1)
}

func runSingle() {
	image, err := ReadImage("test.ppm")
	checkErr(err)

	output := NewImageWithEqualSize(image)
	kernel := NewBlurKernel(10)
	Convolve(image, &output, &kernel)

	err = output.WriteToFile("result.ppm")
	checkErr(err)
}

func fullBenchmark(repetitions int, fileName string) {
	image, err := ReadImage(fileName)
	checkErr(err)
	output := NewImageWithEqualSize(image)

	benchmarkBlur(repetitions, 10, image, &output)
}

func benchmarkBlur(repetitions int, radius int, input *Image, output *Image) {
	kernel := NewBlurKernel(radius)
	benchmarkKernel(repetitions, input, output, &kernel)
}

func benchmarkKernel(repetitions int, input *Image, output *Image, kernel *Kernel) {
	for i := 0; i < repetitions; i++ {
		duration := performRun(input, output, kernel)
		printRuntime(duration, input, kernel)
	}
}

func printRuntime(duration time.Duration, image *Image, kernel *Kernel) {
	fmt.Printf(
		"%v;%vx%v;%v\n",
		kernel.radius, image.width, image.height, duration.Seconds(),
	)
}

func performRun(input *Image, output *Image, kernel *Kernel) time.Duration {
	start := time.Now()
	Convolve(input, output, kernel)
	return time.Since(start)
}
