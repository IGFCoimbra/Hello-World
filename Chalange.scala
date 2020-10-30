package Test

class Testing {
  val a = 1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111;
  var b = 2;
  var C = 3;

  def sqrt(x: Double) = {
    def sqrtIter(guess: Double, x: Double): Double =
      if (isGoodEnough(guess, x)) guess
      else sqrtIter(improve(guess, x), x)
    
    def improve(guess: Double, x: Double) =
      (guess + x / guess) / 2
    
    def isGoodEnough(guess: Double, x: Double) =
      abs(square(guess) - x) < 0.001
    sqrtIter(1.0, x)
  }
}

case class Test(
                 id: String = "",
                 Type: String = "",
                 issue_id: String = "",
                 issue_type: String = "",
                 recipient_id: String = "",
                 recipient_type: String = ""
                )

