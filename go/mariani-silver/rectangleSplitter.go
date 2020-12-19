package main

const partCount = 2

type direction int

const (
	horizontal direction = iota
	vertical
)

type SplittingContext struct {
	splitDirection direction
	original       Rectangle
}

func SplitRectangle(rect Rectangle) [partCount]Rectangle {
	var splitDirection direction
	if rect.height > rect.width {
		splitDirection = horizontal
	} else {
		splitDirection = vertical
	}
	context := SplittingContext{splitDirection, rect}
	return context.split()
}

func (context SplittingContext) split() [partCount]Rectangle {
	parts := [partCount]Rectangle{}

	splittingLength := context.getSplittingLength()
	splitLength := splittingLength / partCount
	remainder := splittingLength % partCount
	offset := uint(0)

	for i := 0; i < partCount; i++ {
		currentSplitLength := splitLength
		if remainder > 0 {
			currentSplitLength++
			remainder--
		}
		splitRectangle := context.splitWith(currentSplitLength, offset)
		parts[i] = splitRectangle

		offset += currentSplitLength
	}
	return parts
}

func (context SplittingContext) getSplittingLength() uint {
	switch context.splitDirection {
	case horizontal:
		return context.original.height
	case vertical:
		return context.original.width
	default:
		panic("Unknown split direction")
	}
}

func (context SplittingContext) splitWith(length, offset uint) Rectangle {
	switch context.splitDirection {
	case horizontal:
		return NewRectangle(
			context.original.x,
			context.original.y+offset,
			context.original.width,
			length,
		)
	case vertical:
		return NewRectangle(
			context.original.x+offset,
			context.original.y,
			length,
			context.original.height,
		)
	default:
		panic("Unknown split direction")
	}
}
