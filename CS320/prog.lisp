(defun initial-prefix (X L)
    "Returns the longest run of prefixed X"
    (if (or (null L) (not (equal X(first L))))
        nil
        (cons (first L) (initial-prefix X (rest L)))))
        
(defun prefix-list (X L)
    "Returns a list of the initial prefix answers for all suffices of L"
    (maplist #'(lambda(M) (initial-prefix  X M)) L))
    
(defun initial-length (X L)
    (mapcar #'length (prefix-list X L)))
    
(defun zero-runs (L)
    (cond ((null L) nil)
            ((not (zerop (first L))) (zero-runs (rest L)))
            ((null (rest L)) (list (list 0)))
            ((not (zerop (second L))) (cons (list 0) (zero-runs (rest (rest L)))))
            (T (cons (cons 0 (first (zerr-runs (rest L))))
                (rest (zero-runs (rest L)))))))
            
(defun max-zero-run-length (L)
    (if (null L)
        0
        (apply #'max (initial-length 0 L))))            

(defun random-from (a b)
    (+ a(random (1+ (- b a)))))
    
(defun random-list (lo hi n)
    (if (zerop n)
        nil
        (cons (random-from lo hi)(random-list lo hi (1- n)))))

(defun calixa-experiment (num-throws)
    (max-zero-run-length (initial-length 0 (random-list 0 1 num-throws))))
    
(defun zero-run-experiment (num-throws num-experiment)
    (if (zerop num-experiment)
        nil
        (cons (calixa-experiment num-throws) (zero-run-experiment num-throws 
            (1- num-experiment)))))




;;; -----------  "My Functions"

            
(defun listNum (k)
        (if (or (null K) (zerop k)) 
                nil
        (cons (list K)(listNum (1- k)))))

(defun sum (list) 
    (if(NULL list)
        0
    (+ (car list)(sum (cdr list)))))
    
(defun divisor (n)
    (divisors-from 1 n))
    
(defun divisors-from (k n)
    (cond ((> k (floor (/ n 2))) nil)
    ((zerop (mod n k))(cons k (divisors-from (+ 1 k) n)))
    (T (divisors-from (+ 1 k) n))))

(defun divisor-sum (n)
    (sum-of (divisor n)))
    
(defun sum-of (L)
    (cond ((null L) 0)
        (T (+( first L)(sum-of (rest L))))))

(defun is-excessive (n)
    (< n (divisor-sum n)))

(defun those-satisfying (attribute L)
    (if (null L)
        nil
    (let ((partial-result (those-satisfying attribute (rest L))))
        (if (funcall attribute (first L))
        (cons (first L) partial-result)
        partial-result))))

(defun count-of(attribute L)
    (length (those-satisfying attribute L)))
    
(defun interval (a b)
    (cond ((= a b) (list a))
        ((< a b) (cons a (interval (1+ a) b)))
        (T (cons a (interval (1- a) b)))))

(defun excessive-list (n)
    (count-of #'is-excessive (interval 1 n)))
    
(defun excessive-range (a n)
    (count-of #'is-excessive (interval a (+ a (1- n)))))
    