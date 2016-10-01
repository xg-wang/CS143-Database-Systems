<html>
<head>
  <title>Calculator</title>
</head>
<body>

<h1>Calculator</h1>
(Ver 0.1 Sep/28/2016 by Yiming Wang & Xingan Wang)<br />
Type an expression in the following box (e.g., 10.5+20*3/25).
<p>
  <form method="GET">
    <input type="text" name="expr">
    <input type="submit" value="Calculate">
  </form>
</p>

<ul>
  <li>Only numbers and +,-,* and / operators are allowed in the expression.
  <li>The evaluation follows the standard operator precedence.
  <li>The calculator does not support parentheses.
  <li>The calculator handles invalid input "gracefully". It does not output PHP error messages.
</ul>

Here are some(but not limit to) reasonable test cases:
<ol>
  <li> A basic arithmetic operation:  3+4*5=23 </li>
  <li> An expression with floating point or negative sign : -3.2+2*4-1/3 = 4.46666666667, 3*-2.1*2 = -12.6 </li>
  <li> Some typos inside operation (e.g. alphabetic letter): Invalid input expression 2d4+1 </li>
</ol>

<?php
// Utilities
  function is_valid($expr) {
    $p_invalid_chars  = '/[^\d\.()\-\+\*\/]/';
    $p_devide_by_zero = '/\/0/';
    $p_redundant_positive_sign = '/[\+\-\*\/]\+/';

    $error_arr = array();
    $expr = preg_replace('/\s+/', '', $expr);
    if (preg_match($p_invalid_chars, $expr)) {
      array_push($error_arr, "contains invalid chars");
    } 
    if (preg_match($p_devide_by_zero, $expr)) {
      array_push($error_arr, "devide by zero");
    } 
    if (preg_match($p_redundant_positive_sign, $expr)) {
      array_push($error_arr, "redundant positive sign");
    }
    return empty($error_arr) ? TRUE : implode(", ", $error_arr);
  }
?>

<?php
  echo "<h2>Result</h2>";
  $expr = $_GET["expr"];
  $error_msg = is_valid($expr);
  if ($error_msg !== TRUE) {
    echo "Invalid input: " . $expr . ".<br>error: " . $error_msg . "."; 
    echo "<br>Please try again :-)";
  } else {
    $expr = preg_replace('/(\d\-)(\-\d)/', '$1 $2', $expr);
    $error = eval("\$res=$expr;");
    if ($error === FALSE) {
      echo "Invalid input: " . $expr . ".";
      echo "<br>Please try again :-)";
    } else {
      echo $expr . " = " . $res;
    }
  }
?>