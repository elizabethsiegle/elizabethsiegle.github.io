-- Lab02.hs
-- Name: Lizzie Siegle
--
--
-- Read the problems below.
-- When you are done, upload your work in the lab02 directory at our
-- classwork repo, at https://github.com/bmc-cs380/classwork
-- Please rename the file before upload so you don't clobber your
-- classmates' work!
--

{-# OPTIONS -Wall -Wno-type-defaults #-}

module Lab02 where

-- The "Prelude" is the module automatically imported into all Haskell
-- programs. But we'll be redefining some Prelude functions in this file,
-- so we don't want GHCi to get confused by having both your and the
-- Prelude's in scope. So we hide some imports.
-- (By the way, the Prelude is documented here:
--  http://hackage.haskell.org/package/base-4.9.1.0/docs/Prelude.html)
import Prelude hiding (all, reverse, takeWhile, zip, concat, concatMap)

import Test.HUnit

--------------------------------------------------------------------------------
-- The code in this section works -- the test passes. (Run runTestTT testStyle
-- to see this.) But the style is awful. Fix.

testStyle :: Test
testStyle = "testStyle" ~:
   TestList [tabc, treverse]

--abc x y z =
--  if x then 
--    if y 
--      then True 
--      else
--       if (x && z) 
--        then True 
--        else False
--  else False
abc:: Bool -> Bool -> Bool -> Bool
abc x y z   | x && y      = True
            | x && (x&&z) = True
            | otherwise   = False



tabc :: Test
tabc = "abc" ~: TestList [abc True False True ~?= True,
                          abc True False False ~?= False,
                          abc False True True ~?= False]

reverse:: [a] -> [a]
reverse [] = []
reverse (x : xs) = reverse xs ++ [x]


treverse :: Test -- reverse [] ~? ([] :: [Int])
treverse = "reverse" ~: TestList [reverse [3,2,1] ~?= [1,2,3],
                                  reverse [1]     ~?= [1] ]

-- ------------------------------------------------------------------------------
-- Below are many functions over lists. Write the functions and tests over those
-- functions.

testLists :: Test
--tfind
testLists = "testLists" ~: TestList [ttakeWhile, ttall, tmap2, tzip, ttranspose, tconcat]
-- returns the longest prefix (possibly empty) of xs of elements
-- that satisfy p:
-- For example,
--     takeWhile (< 3) [1,2,3,4,1,2,3,4] == [1,2]
--     takeWhile (< 9) [1,2,3] == [1,2,3]
--     takeWhile (< 0) [1,2,3] == []

takeWhile:: (a->Bool) -> [a] -> [a]
takeWhile _ [] = []
takeWhile p (x: xs)
    | p x       = x: takeWhile p xs
    | otherwise = []
   
ttakeWhile :: Test
ttakeWhile = 
  "takeWhile" ~: TestList [
  "takeWhile (=< -2) [-5, -2, 6]" ~: takeWhile (<2) [(-5)::Int, (-2), 6] ~?= [(-5), (-2)],
  "takeWhile (> 100) [103, 101, 86, -3]" ~: takeWhile (>100) [103::Int, 101, 86, (-3)] ~?= [103, 101],
  "takeWhile (>= 5) [4, 0, -2]" ~: takeWhile(>= 5) [4::Int, 0, (-2)] ~?= [] 
  ] --fin



-- find pred lst returns the first element of the list that
-- satisfies the predicate. Because no element may do so, the
-- answer is returned in a "Maybe".
-- for example:
--     find odd [0,2,3,4] returns Just 3
--find:: (a->Bool) -> [a] -> Maybe a
--find _ [] = Nothing
--find p (x:xs)
--    |  p x      = Just x
--    | otherwise = find p xs

--tfind :: Test
--tfind = 
--  "tfind" ~: TestList [
--  "tfind (< 5) [4 5 6 3]" ~:find (< 5) [4::Int, 5, 6, 3] ~?= [4],
--  "tfind (>= 3) [9 8 7 6 3 2" ~: find (>= 3) [9::Int, 8, 7, 6, 3, 2] ~?= [3],
--  "tfind (< (-2)) [-1 -3 9 (-4)]" ~:find (< (-2)) [(-1)::Int, (-3), 9, (-4)] ~?= [(-3)]
--    ]


-- all pred lst returns False if any element of lst fails to satisfy
-- pred and True otherwise.
-- for example:
--    all odd [1,2,3] returns False
tall:: (a->Bool) -> [a] -> Bool
tall _ [] = True
tall p (x: xs) = (p x) && (tall p xs)


ttall :: Test
ttall = "ttall" ~: TestList [
  "ttall (=< -2) [-5, -2, 6]" ~: tall (<2) [(-5)::Int, (-2), 6] ~?= False,
  "ttall ( > 3) [8, 7, 6, 5, 4]" ~: tall (>3) [8::Int, 7, 6, 5, 4] ~?= True
 
  ] --fin


-- map2 f xs ys returns the list obtained by applying f to
-- to each pair of corresponding elements of xs and ys. If
-- one list is longer than the other, then the extra elements
-- are ignored.
-- i.e.
--   map2 f [x1, x2, ..., xn] [y1, y2, ..., yn, yn+1]
--        returns [f x1 y1, f x2 y2, ..., f xn yn]
--
-- NOTE: map2 is called zipWith in the standard library.
map2 :: (a -> b -> c) -> [a] -> [b] -> [c]  
map2 _ _ [] = []
map2 _ [] _ = []
map2 f (x:xs) (y:ys) = (f x y) : map2 f xs ys

tmap2 :: Test
tmap2 = "map2" ~: TestList [ 
    "map2 -" ~:map2 (-) [4, 5, 6] [1, 2, 3] ~?= [3, 3, 3]
    , "map2 (-1)* +" ~: map2 (\x y -> x*(-1) + y) [0, 1, 2, 3] [4, 5, 6, 7] ~?= [4, 4, 4, 4]
    ]



-- zip takes two lists and returns a list of corresponding pairs. If
-- one input list is shorter, excess elements of the longer list are
-- discarded.
-- for example:
--    zip [1,2] [True] returns [(1,True)]
zip1:: [a] -> [b] -> [(a, b)]
zip1 xs ys = [(xs!!i, ys!!i) | i <- [0..(min (length xs) (length ys))-1]]
tzip :: Test
tzip = "zip" ~: TestList [
    "zip1" ~: zip1 [1, 3, 5, 7] [2, 4, 6, 8] ~?= [(1, 2), (3, 4), (5, 6), (7, 8)]
    , "zip1 2" ~: zip1 [9, 8, 7, 6, 5] ['a', 'b', 'c'] ~?= [(9, 'a'), (8, 'b'), (7, 'c')]
    ]


-- transpose  (WARNING: this one is tricky!)

-- The transpose function transposes the rows and columns of its argument.
-- If the inner lists are not all the same length, then the extra elements
-- are ignored. Note, this is not the same behavior as the library version
-- of transpose.

-- for example:
--    transpose [[1,2,3],[4,5,6]] returns [[1,4],[2,5],[3,6]]
--    transpose  [[1,2],[3,4,5]] returns [[1,3],[2,4]]
transpose:: [[a]]->[[a]]
transpose []      = []
transpose xs      = (map head xs) : transpose (map tail xs)
ttranspose :: Test
ttranspose = "transpose" ~: TestList [
                       transpose [[1,2], [3,4,5]]   ~?=  [[1,3],[2,4]]
                       --, transpose [[(-4),(-3),(-2)], [(-7),(-6),(-5)]] ~?= [[-4,-7],[-3,-6],[-2,]]
                       ]


-- concat

-- The concatenation of all of the elements of a list of lists
-- for example:
--    concat [[1,2,3],[4,5,6],[7,8,9]] returns [1,2,3,4,5,6,7,8,9]
concat1:: [[a]] -> [a]
concat1 [] = []
concat1 ([]:ys) = concat1 ys
concat1 ((x:xs):ys) = x : concat1 (xs:ys)
tconcat :: Test
tconcat = "concat" ~: TestList [
    "concat" ~: concat1 [[1, 2, 3], [4, 5, 6], [7, 8, 9]] ~?= [1, 2, 3, 4, 5, 6, 7, 8, 9]
    , "tconcat2" ~: concat1 [['a', 'b'], ['c', 'd', 'e']] ~?= ['a', 'b', 'c', 'd', 'e']
    ]


-- concatMap

-- Map a function over all the elements of the list and concatenate the results.
-- for example:
--    concatMap (\x -> [x,x+1,x+2]) [1,2,3]  returns [1,2,3,2,3,4,3,4,5]
concatMap1 :: (a->[b]) -> [a] -> [b]
concatMap1 _[]      = []
concatMap1 f (x:xs) = f x ++ (concatMap1 f xs)
tconcatMap :: Test
tconcatMap = "concatMap" ~: TestList [
    "tconcatMap1" ~: concatMap1 (\x -> [x,x+1,x+2]) [1,2,3] ~?= [1,2,3,2,3,4,3,4,5]
    ,"tconcatMap2" ~: concatMap1 (\x -> [x/2]) [12,10,8] ~?= [6, 5, 4]
    --, "tconcatMap3" ~: concatMap1 (++ "!") ["Excited", "happy", "fun", "funny", "LOL"] ~?= ["Excited","!", "happy", "!", "fun", "!", "funny", "!", "LOL", "!"]
    ]

all_tests = TestList [
    tconcatMap,
    tconcat,
    ttranspose,
    tzip,
    tmap2,
    ttall,
    ttakeWhile,
    tabc,
    treverse
    ]


    --assert $ pass @=? cases s0 - (errors s0 + failures s0)