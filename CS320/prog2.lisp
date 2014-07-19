;;CS320
;;Lisp Assignment 2

;;Explores excessive numbers and tackles the Calixta Problem

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;OTHER FUNCTIONS (EXCESSIVE, INTERVAL, LISTS);;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun consecutive-list (a b)
    "Answer the list a...b if a<=b else a downTo b"
    (if (= a b)
        (list a)
        (if (< a b)
            (cons a (consecutive-list (+ 1 a) b))
            (cons a (consecutive-list (- a 1) b)))))
              
            
(defun is-excessive (n)
    (< n (divisor-sum n)))
    

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
    
    
(defun interval (a b)
    (cond ((= a b) (list a))
        ((< aa b) (cons a (interval (+ 1 a) b)))
        (T (cons a (interval (- 1 a) b)))))
        
(defun excessive-list (n)
    (those-satisfying #'is-excessive (interval 1 n)))
    
    
(defun excessive-range-list (a n)
    (those-satisfying #'is-excessive (interval a (+ a (- 1 n)))))
    
    
(defun those-satisfying (attribute L)
"Answer the list of entries of L satisfying the predicate 'attribute'"
    (if (null L)
    nil
    (let ((partial-result (those-satisfying attribute (rest L))))
        (if (funcall attribute (first L))
        (cons (first L) partial-result)
            partial-result))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;            
;;;;CALIXTA PROBLEM;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun initial-prefix (x L)
    "Returns longest run of the prefixed x"
    (if (or (null L) (not (equal x(first L))))
        nil
        (cons (first L) (initial-prefix x (rest L)))))
        

(defun prefix-list (x L)
    "Returns list of the intial prefix answers in L"
    (maplist #'(lambda(M) (initial-prefix X M)) L))
    
    
(defun initial-length (x L)
    (mapcar #'length (prefix-list x L)))
    

(defun zero-runs (L)
    (cond ((null L) nil)
        ((not (zerop (first L))) (zero-runs (rest L)))
        ((null (rest L)) (list (list 0)))
        ((not (zerop (second L))) (cons (list 0) 
                (zero-runs (rest (rest L)))))
        (T (cons (cons 0 (first (zero-runs (rest L))))
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
        (cons (random-from lo hi) (random-list lo hi (1- n)))))
        
        
(defun calixta-zero-experiment (num-throws)
    (max-zero-run-length (initial-length 0 (random-list 0 1 num-throws))))
    

(defun zero-run-experiment (num-throws num-experiment)
    (if (zerop num-experiment)
        nil
        (cons (calixta-zero-experiment num-throws) 
            (zero-run-experiment num-throws (1- num-experiment))))) 
            
            
(defun one-runs (L)
    (cond ((null L) nil)
        ((not (= 1 (first L))) (one-runs (rest L)))
        ((null (rest L)) (list (list 1)))
        ((not (= 1 (second L))) (cons (list 1) 
                (one-runs (rest (rest L)))))
        (T (cons (cons 1 (first (one-runs (rest L))))
            (rest (one-runs (rest L)))))))
            
            
(defun max-one-run-length (L)
    (if (null L)
        0
        (apply #'max (initial-length 1 L))))
        
        
(defun calixta-one-experiment (num-throws)
    (max-one-run-length (random-list 0 1 num-throws)))
    

(defun one-run-experiment (num-throws num-experiment)
    (if (zerop num-experiment)
        nil
        (cons (calixta-one-experiment num-throws) 
            (one-run-experiment num-throws (1- num-experiment)))))
            
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;AVERAGE,STDEV;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            
(defun sum(L)
    (if (null L) 0
        (+ (car L) (sum (cdr L)))))
        

(defun avg(L)
    (if (null L) 0
        (/ (sum L) (length L))))


;;Had trouble figuring this out, found example here:
;;https://github.com/acmeism/RosettaCodeData/blob
;;    /master/Task/Standard-deviation/Common-Lisp/standard-deviation-2.lisp

(defun standard-deviation (L)
(let ((mean (avg L)))
(sqrt (* (/ 1.0d0 (length L))
(reduce #'+ L
:key (lambda (x)
(expt (- x mean) 2)))))))