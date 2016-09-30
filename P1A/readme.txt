### PHP Web Calculator

In this part of the project you will familiarize yourself with Apache2/PHP by building a small Web calculator application in PHP.

* **Step 1:** Review the [W3CSchools PHP tutorial](http://www.w3schools.com/php/php_intro.asp) to learn basic PHP. Please read at least up to "PHP $\_POST" page of the tutorial. You can test the examples in the tutorial by creating a php page in the`${HOME}/www/` directory in the guest OS, which is aliased (or symbolic linked) to the VirtualBox shared directory. All files in `${HOME}/www/` are served by the guest Apache server and are accessible at<http://localhost:1438/~cs143/> from your host browser. Note that `${HOME}` is common Unix notation to refer to your home directory, which is `/home/cs143/` in our setup.
* **Step 2:** Play with our demo calculator [here](http://oak.cs.ucla.edu/classes/cs143/project/demo/calc/) (access restricted to username 'project' and password 'demo') to understand what it does.
* **Step 3:** Implement your own calculator and make it available at <http://localhost:1438/~cs143/calculator.php>. At the minimum, your calculator application should satisfy the following requirements.

  1. It should support +, -, \* and / operators and the evaluation of the input should follow the standard operator precedence (i.e., the operators should be left-associative and / and \* operators have precedence over + and -).
  2. It should take both interger (like 1234) and real (like 123.45) numbers.
  3. It *does not* need to support parentheses. (As a side note, in case you took the compiler class before, you may remember that the correct handling of nested-parentheses requires more expressive power than regular expression, like context-free grammar.)
  4. It should handle any errors gracefully. For example, even if the user input is invalid expression, the result page should not display raw PHP error message.
  5. Your calculator should be implemented as a *single* .php page. Make sure that it does not include and/or read any other file (like CSS file, images or other HTML files).
  6. For all links in your calculator, including form actions, you should use ***relative URLs*** instead of absolute. The reason is simple: depending on where we run your Web calculator, the hostname and the path of your calculator may change. So if you use absolute URL, you calculator may *break* when we try to grade it and we may have to give zero point for your work!
  7. The calculator.php is more like a question-answering interface, and therefore you should use **HTTP GET** protocol to process the user input as [suggested by W3C](http://www.w3.org/2001/tag/doc/whenToUseGet.html#checklist).

  In implementing your calculator, you may find the PHP functions, `[preg\_match()](http://us2.php.net/preg_match)` and `[eval()](http://us2.php.net/eval)` helpful. If you are not familiar with regular expression, read a tutorial on regular expression like [Mastering regular expression in PHP](http://www.ibm.com/developerworks/library/os-php-regex1/index.html). If you are not familiar with the HTML input forms, you may also find our [tutorial on PHP input handling](http://oak.cs.ucla.edu/classes/cs143/project/php/php_input.html) helpful.

Throughly test your code and make sure that it meets the above minimum requirements and runs correctly on our virtual machine. Note that this part of the project will be graded based on the functionality not on the look or style. As long as you meet the minimum requirements, you will get full credit for this project.