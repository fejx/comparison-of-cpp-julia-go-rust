package main

type Rectangle struct {
	x, y, width, height, endX, endY uint
}

func NewRectangle(x, y, width, height uint) Rectangle {
	return Rectangle{
		x, y,
		width, height,
		x + width, y + height,
	}
}
