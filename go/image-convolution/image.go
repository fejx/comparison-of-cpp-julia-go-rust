package main

import (
	"bufio"
	"errors"
	"os"
	"strconv"
)

type Color uint8
type Pixel [3]Color

type Image struct {
	width             int
	height            int
	totalSize         int
	maximumColorValue Color
	pixels            []Pixel
}

func NewImage(width int, height int, maximumColorValue Color) Image {
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

func ReadImage(fileName string) (*Image, error) {
	file, err := os.Open(fileName)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	reader := bufio.NewReader(file)

	header, err := ReadStringExcludingNewline(reader)
	if err != nil {
		return nil, err
	}
	if header != "P6" {
		return nil, errors.New("not a ppm file")
	}
	widthString, err := ReadStringExludingDelim(reader, ' ')
	if err != nil {
		return nil, err
	}
	heightString, err := ReadStringExcludingNewline(reader)
	if err != nil {
		return nil, err
	}
	maximumColorValueString, err := ReadStringExcludingNewline(reader)
	if err != nil {
		return nil, err
	}
	width, err := strconv.Atoi(widthString)
	if err != nil {
		return nil, err
	}
	height, err := strconv.Atoi(heightString)
	if err != nil {
		return nil, err
	}
	maximumColorValue, err := strconv.Atoi(maximumColorValueString)
	if err != nil {
		return nil, err
	}
	if maximumColorValue > 255 {
		return nil, errors.New("maximum color values above 255 are not supported")
	}

	image := NewImage(width, height, Color(maximumColorValue))
	pixels := image.pixels

	for i := 0; i < image.totalSize; i++ {
		pixel := &pixels[i]
		for c := 0; c < 3; c++ {
			rawByte, err := reader.ReadByte()
			if err != nil {
				return nil, err
			}
			color := Color(rawByte)
			pixel[c] = color
		}
	}

	return &image, nil
}

func ReadStringExcludingNewline(reader *bufio.Reader) (string, error) {
	return ReadStringExludingDelim(reader, '\n')
}

func ReadStringExludingDelim(reader *bufio.Reader, delim byte) (string, error) {
	readStr, err := reader.ReadString(delim)
	if err != nil {
		return "", err
	}
	last := len(readStr) - 1
	return readStr[:last], nil
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
	widthString := strconv.Itoa(image.width)
	_, err = writer.WriteString(widthString)
	if err != nil {
		return err
	}
	_, err = writer.WriteString(" ")
	if err != nil {
		return err
	}
	heightString := strconv.Itoa(image.height)
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

func (image *Image) GetOr(x int, y int, fallback *Pixel) *Pixel {
	if 0 > x || x >= image.width || 0 > y || y >= image.height {
		return fallback
	} else {
		i := image.CombineIndex(x, y)
		return &image.pixels[i]
	}
}

func (image *Image) Get(x int, y int) *Pixel {
	i := image.CombineIndex(x, y)
	return &image.pixels[i]
}

func (image *Image) CombineIndex(x int, y int) int {
	return y*image.width + x
}
