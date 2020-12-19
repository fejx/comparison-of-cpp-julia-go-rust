package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
)

type Color uint8
type Pixel [3]Color

type Image struct {
	width             uint
	height            uint
	totalSize         uint
	maximumColorValue Color
	pixels            []Pixel
}

func NewImage(width uint, height uint) Image {
	return NewImageWithCustomColor(width, height, 255)
}

func NewImageWithCustomColor(width uint, height uint, maximumColorValue Color) Image {
	totalSize := width * height
	pixels := make([]Pixel, totalSize)

	return Image{
		width,
		height,
		totalSize,
		maximumColorValue,
		pixels,
	}
}

func (image *Image) WriteToFile(fileName string) error {
	file, err := os.Create(fileName)
	if err != nil {
		return err
	}
	defer file.Close()

	writer := bufio.NewWriter(file)
	defer writer.Flush()
	_, err = writer.WriteString("P6\n")
	if err != nil {
		return err
	}
	widthString := uintToString(image.width)
	_, err = writer.WriteString(widthString)
	if err != nil {
		return err
	}
	_, err = writer.WriteString(" ")
	if err != nil {
		return err
	}
	heightString := uintToString(image.height)
	_, err = writer.WriteString(heightString)
	if err != nil {
		return err
	}
	_, err = writer.WriteString("\n")
	if err != nil {
		return err
	}
	maximumColorValueString := strconv.Itoa(int(image.maximumColorValue))
	_, err = writer.WriteString(maximumColorValueString)
	if err != nil {
		return err
	}
	_, err = writer.WriteString("\n")
	if err != nil {
		return err
	}

	for _, pixel := range image.pixels {
		for _, channel := range pixel {
			b := byte(channel)
			err = writer.WriteByte(b)
			if err != nil {
				return err
			}
		}
	}

	return nil
}

func (image *Image) GetOr(x, y uint, fallback *Pixel) *Pixel {
	if 0 > x || x >= image.width || 0 > y || y >= image.height {
		return fallback
	} else {
		i := image.CombineIndex(x, y)
		return &image.pixels[i]
	}
}

func (image *Image) Get(x, y uint) *Pixel {
	i := image.CombineIndex(x, y)
	return &image.pixels[i]
}

func (image *Image) CombineIndex(x, y uint) uint {
	return y*image.width + x
}

func uintToString(uint uint) string {
	return fmt.Sprint(uint)
}
