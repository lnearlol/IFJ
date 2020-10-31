package main

func main() {
  a := 0
	a, _ = inputi()
  if a < 0 {
  } else {
    vysl := 1
    for ; a > 0; a = a - 1 {
      vysl = vysl * a
    }
  }
}