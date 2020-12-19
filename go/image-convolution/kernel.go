package main

type Kernel struct {
	radius    int
	width     int
	totalSize int
	matrix    []float32
}

func NewKernel(radius int) Kernel {
	width := radius*2 + 1
	totalSize := width * width
	matrix := make([]float32, totalSize)
	return Kernel{
		radius,
		width,
		totalSize,
		matrix,
	}
}

func NewBlurKernel(radius int) Kernel {
	k := NewKernel(radius)
	value := float32(1) / float32(k.totalSize)
	matrix := k.matrix
	for i := 0; i < k.totalSize; i++ {
		matrix[i] = value
	}
	return k
}

func (kernel Kernel) Get(x int, y int) float32 {
	i := kernel.CombineIndex(x, y)
	return kernel.matrix[i]
}

func (kernel Kernel) CombineIndex(x int, y int) int {
	return y*kernel.width + x
}
