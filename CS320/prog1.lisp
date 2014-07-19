(defun quality (x)
    (cond ((not (numberp x)) "non-number") ; type test!
        ((> x 0) "big")
        ((= x 0) "zero")
        (T "hmmm"))) ; T = "true", i.e. 'otherwise' here


(defun is-prime (n)
    (cond ((< n 0) (is-prime (- n))) ; reverse the sign
        ((<= n 1) nil) ; 0, 1 not prime
        ((<= n 3) T) ; 2, 3 are prime
        ((= 0 (mod n 2)) nil) ; evens > 2 not prime
        (T (do ((k 3 (+ k 2))) ; a loop, so not very
            ;'functional'
        ((> k (floor (sqrt n))) T) ; loop exit, so prime
        (when (= 0 (mod n k)) ; bad case: not prime
        (return nil))))))


(defun those-satisfying (attribute L)
"Answer the list of entries of L satisfying the predicate 'attribute'"
    (if (null L)
    nil
    (let ((partial-result (those-satisfying attribute (rest L))))
        (if (funcall attribute (first L))
        (cons (first L) partial-result)
            partial-result))))
            
            
(defun count-of(attribute L)
    "answer number of entries in L satisfying attribute"
    (length (those-satisfying attribute L)))
    
            
(defun next-satisfying (attribute x)
    "Answer the next integer > x satisfying attribute.
    NB: An infinite loop if none exist"
    (if (funcall attribute (1+ x)) 
    (1+ x)
    (next-satisfying attribute (1+ x))))


(defun interval (a b)
"answer a..b or a downto b if a>b"
    (cond ((= a b) (list a))
    ((< a b) (cons a (interval (1+ a) b)))))



(defun fibo(n)
"fibonacci sequence"
    (if (<= n 1)
        n
        (+ (fibo (- n 1)) (fibo (- n 2)))))
        

(defun first-prime-greater-than (x)
"finding the next prime after x"
    (if (is-prime (+ x 1)
        (+ x 1)
            (first-prime-greater-than (+ x 1)))))
            

(defun consecutive-list (a b)
    "Answer the list a...b if a<=b else a downTo b"
    (if (= a b)
        (list a)
        (if (< a b)
            (cons a (consecutive-list (+ 1 a) b))
            (cons a (consecutive-list (- a 1) b)))))
                
        
(defun random-list(lo hi n)
    "Answer a random list of n integers in the range lo...hi"
    (if (zerop n)
        nil
        (cons (+ lo(random (+ 1 (- hi lo))))
              (random-list lo hi (- n 1)))))
              
            
(defun is-perfect (n)
    (= n (divisor-sum n)))
    

(defun divisor-sum (n)
    (sum-of (divisors n)))
    

(defun sum-of (L)
    (cond ((null L) 0)
        (T (+ (first L) (sum-of (rest L))))))
        
        
(defun divisors (n)
    (divisors-from 1 n))
    
    
(defun divisors-from (k n)
    (cond ((> k (floor (/ n 2))) nil)
    ((zerop (mod n k)) (cons k (divisors-from (+ 1 k) n)))
    (T (divisors-from (+ 1 k) n))))        
    
    
(defun sum(L)
    (if (null L) 0
        (+ (car L) (sum (cdr L)))))
        

(defun avg(L)
    (if (null L) 0
        (/ (sum L) (length L))))