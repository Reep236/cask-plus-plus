[B] - Builtin 
[X] - Complete
[I] - In Progress
[N] - Not Planned
[ ] - TODO

[B] data Bool
[B]     = False
[B]     | True
[B] (&&) :: Bool -> Bool -> Bool
[B] (||) :: Bool -> Bool -> Bool
[B] not :: Bool -> Bool
[N] otherwise :: Bool 
[X] data Maybe a
[X] = Nothing
[X] | Just a
[X] maybe :: b -> (a -> b) -> Maybe a -> b
[X] data Either a b
[X] = Left a
[X] | Right b
[X] either :: (a -> c) -> (b -> c) -> Either a b -> c
[X] data Ordering
[X] = LT
[X] | EQ
[X] | GT
[B] data Char
[B] type String = [Char]
[X] fst :: (a, b) -> a
[X] snd :: (a, b) -> b
[X] curry :: ((a, b) -> c) -> a -> b -> c
[X] uncurry :: (a -> b -> c) -> (a, b) -> c
[X] class Eq a where
[B] (==) :: a -> a -> Bool
[B] (/=) :: a -> a -> Bool
[X] class Eq a => Ord a where
[X] compare :: a -> a -> Ordering
[X] (<) :: a -> a -> Bool
[X] (<=) :: a -> a -> Bool
[X] (>) :: a -> a -> Bool
[X] (>=) :: a -> a -> Bool
[X] max :: a -> a -> a
[X] min :: a -> a -> a
[X] class Enum a where
[X] succ :: a -> a
[X] pred :: a -> a
[X] toEnum :: Int -> a
[X] fromEnum :: a -> Int
[X] enumFrom :: a -> [a]
[X] enumFromThen :: a -> a -> [a]
[X] enumFromTo :: a -> a -> [a]
[X] enumFromThenTo :: a -> a -> a -> [a]
[X] class Bounded a where
[X] minBound :: a
[X] maxBound :: a
[X] data Int
[X] data Integer
[X] data Float
[X] data Double
[ ] type Rational = Ratio Integer
[X] data Word
[X] class Num a where
[X] (+) :: a -> a -> a
[X] (-) :: a -> a -> a
[X] (*) :: a -> a -> a
[X] negate :: a -> a
[X] abs :: a -> a
[X] signum :: a -> a
[X] fromInteger :: Integer -> a
[ ] class (Num a, Ord a) => Real a where
[ ] toRational :: a -> Rational
[ ] class (Real a, Enum a) => Integral a where
[ ] quot :: a -> a -> a
[ ] rem :: a -> a -> a
[ ] div :: a -> a -> a
[ ] mod :: a -> a -> a
[ ] quotRem :: a -> a -> (a, a)
[ ] divMod :: a -> a -> (a, a)
[ ] toInteger :: a -> Integer
[ ] class Num a => Fractional a where
[ ] (/) :: a -> a -> a
[ ] recip :: a -> a
[ ] fromRational :: Rational -> a
[ ] class Fractional a => Floating a where
[ ] pi :: a
[ ] exp :: a -> a
[ ] log :: a -> a
[ ] sqrt :: a -> a
[ ] (**) :: a -> a -> a
[ ] logBase :: a -> a -> a
[ ] sin :: a -> a
[ ] cos :: a -> a
[ ] tan :: a -> a
[ ] asin :: a -> a
[ ] acos :: a -> a
[ ] atan :: a -> a
[ ] sinh :: a -> a
[ ] cosh :: a -> a
[ ] tanh :: a -> a
[ ] asinh :: a -> a
[ ] acosh :: a -> a
[ ] atanh :: a -> a
[ ] class (Real a, Fractional a) => RealFrac a where
[ ] properFraction :: Integral b => a -> (b, a)
[ ] truncate :: Integral b => a -> b
[ ] round :: Integral b => a -> b
[ ] ceiling :: Integral b => a -> b
[ ] floor :: Integral b => a -> b
[ ] class (RealFrac a, Floating a) => RealFloat a where
[ ] floatRadix :: a -> Integer
[ ] floatDigits :: a -> Int
[ ] floatRange :: a -> (Int, Int)
[ ] decodeFloat :: a -> (Integer, Int)
[ ] encodeFloat :: Integer -> Int -> a
[ ] exponent :: a -> Int
[ ] significand :: a -> a
[ ] scaleFloat :: Int -> a -> a
[ ] isNaN :: a -> Bool
[ ] isInfinite :: a -> Bool
[ ] isDenormalized :: a -> Bool
[ ] isNegativeZero :: a -> Bool
[ ] isIEEE :: a -> Bool
[ ] atan2 :: a -> a -> a
[ ] subtract :: Num a => a -> a -> a
[ ] even :: Integral a => a -> Bool
[ ] odd :: Integral a => a -> Bool
[ ] gcd :: Integral a => a -> a -> a
[ ] lcm :: Integral a => a -> a -> a
[ ] (^) :: (Num a, Integral b) => a -> b -> a
[ ] (^^) :: (Fractional a, Integral b) => a -> b -> a
[ ] fromIntegral :: (Integral a, Num b) => a -> b
[ ] realToFrac :: (Real a, Fractional b) => a -> b
[X] class Semigroup a where
[X] (<>) :: a -> a -> a
[X] class Semigroup a => Monoid a where
[X] mempty :: a
[X] mappend :: a -> a -> a
[X] mconcat :: [a] -> a
[X] class Functor (f :: Type -> Type) where
[X] fmap :: (a -> b) -> f a -> f b
[X] (<$) :: a -> f b -> f a
[X] (<$>) :: Functor f => (a -> b) -> f a -> f b
[X] class Functor f => Applicative (f :: Type -> Type) where
[X] pure :: a -> f a
[X] (<*>) :: f (a -> b) -> f a -> f b
[X] liftA2 :: (a -> b -> c) -> f a -> f b -> f c
[X] (*>) :: f a -> f b -> f b
[X] (<*) :: f a -> f b -> f a
[X] class Applicative m => Monad (m :: Type -> Type) where
[X] (>>=) :: m a -> (a -> m b) -> m b
[X] (>>) :: m a -> m b -> m b
[X] return :: a -> m a
[N] class Monad m => MonadFail (m :: Type -> Type) where
[N] fail :: String -> m a
[X] mapM_ :: (Foldable t, Monad m) => (a -> m b) -> t a -> m ()
[X] sequence_ :: (Foldable t, Monad m) => t (m a) -> m ()
[X] (=<<) :: Monad m => (a -> m b) -> m a -> m b
[X] class Foldable (t :: Type -> Type) where
[X] foldMap :: Monoid m => (a -> m) -> t a -> m
[X] foldr :: (a -> b -> b) -> b -> t a -> b
[X] foldl :: (b -> a -> b) -> b -> t a -> b
[N] foldl' :: (b -> a -> b) -> b -> t a -> b
[X] foldr1 :: (a -> a -> a) -> t a -> a
[X] foldl1 :: (a -> a -> a) -> t a -> a
[X] elem :: Eq a => a -> t a -> Bool
[X] maximum :: Ord a => t a -> a
[X] minimum :: Ord a => t a -> a
[X] sum :: Num a => t a -> a
[X] product :: Num a => t a -> a
[X] class (Functor t, Foldable t) => Traversable (t :: Type -> Type) where
[X] traverse :: Applicative f => (a -> f b) -> t a -> f (t b)
[X] sequenceA :: Applicative f => t (f a) -> f (t a)
[X] mapM :: Monad m => (a -> m b) -> t a -> m (t b)
[X] sequence :: Monad m => t (m a) -> m (t a)
[X] id :: a -> a
[X] const :: a -> b -> a
[X] (.) :: (b -> c) -> (a -> b) -> a -> c
[X] flip :: (a -> b -> c) -> b -> a -> c
[N] ($) :: (a -> b) -> a -> b | !!! Implemented as part of `partial`
[X] until :: (a -> Bool) -> (a -> a) -> a -> a
[ ] asTypeOf :: a -> a -> a
[ ] error :: HasCallStack => [Char] -> a
[ ] errorWithoutStackTrace :: [Char] -> a
[ ] undefined :: HasCallStack => a
[ ] seq :: a -> b -> b
[ ] ($!) :: (a -> b) -> a -> b
[X] map :: (a -> b) -> [a] -> [b]
[X] (++) :: [a] -> [a] -> [a]
[X] filter :: (a -> Bool) -> [a] -> [a]
[X] head :: HasCallStack => [a] -> a
[X] last :: HasCallStack => [a] -> a
[X] tail :: HasCallStack => [a] -> [a]
[X] init :: HasCallStack => [a] -> [a]
[X] (!!) :: HasCallStack => [a] -> Int -> a
[X] null :: Foldable t => t a -> Bool
[X] length :: Foldable t => t a -> Int
[X] reverse :: [a] -> [a]
[X] and :: Foldable t => t Bool -> Bool
[X] or :: Foldable t => t Bool -> Bool
[X] any :: Foldable t => (a -> Bool) -> t a -> Bool
[X] all :: Foldable t => (a -> Bool) -> t a -> Bool
[X] concat :: Foldable t => t [a] -> [a]
[X] concatMap :: Foldable t => (a -> [b]) -> t a -> [b]
[X] scanl :: (b -> a -> b) -> b -> [a] -> [b]
[X] scanl1 :: (a -> a -> a) -> [a] -> [a]
[X] scanr :: (a -> b -> b) -> b -> [a] -> [b]
[X] scanr1 :: (a -> a -> a) -> [a] -> [a]
[N] iterate :: (a -> a) -> a -> [a]
[N] repeat :: a -> [a]
[X] replicate :: Int -> a -> [a]
[N] cycle :: HasCallStack => [a] -> [a]
[X] take :: Int -> [a] -> [a]
[X] drop :: Int -> [a] -> [a]
[X] takeWhile :: (a -> Bool) -> [a] -> [a]
[X] dropWhile :: (a -> Bool) -> [a] -> [a]
[X] span :: (a -> Bool) -> [a] -> ([a], [a])
[X] break :: (a -> Bool) -> [a] -> ([a], [a])
[X] splitAt :: Int -> [a] -> ([a], [a])
[X] notElem :: (Foldable t, Eq a) => a -> t a -> Bool
[X] lookup :: Eq a => a -> [(a, b)] -> Maybe b
[X] zip :: [a] -> [b] -> [(a, b)]
[X] zip3 :: [a] -> [b] -> [c] -> [(a, b, c)]
[X] zipWith :: (a -> b -> c) -> [a] -> [b] -> [c]
[X] zipWith3 :: (a -> b -> c -> d) -> [a] -> [b] -> [c] -> [d]
[X] unzip :: [(a, b)] -> ([a], [b])
[X] unzip3 :: [(a, b, c)] -> ([a], [b], [c])
[ ] lines :: String -> [String]
[ ] words :: String -> [String]
[ ] unlines :: [String] -> String
[ ] unwords :: [String] -> String
[ ] type ShowS = String -> String
[X] class Show a where
[ ] showsPrec :: Int -> a -> ShowS
[X] show :: a -> String
[ ] showList :: [a] -> ShowS
[ ] shows :: Show a => a -> ShowS
[ ] showChar :: Char -> ShowS
[ ] showString :: String -> ShowS
[ ] showParen :: Bool -> ShowS -> ShowS
[ ] type ReadS a = String -> [(a, String)]
[X] class Read a where
[ ] readsPrec :: Int -> ReadS a
[ ] readList :: ReadS [a]
[ ] reads :: Read a => ReadS a
[ ] readParen :: Bool -> ReadS a -> ReadS a
[X] read :: Read a => String -> a
[ ] lex :: ReadS String
[X] data IO a
[X] putChar :: Char -> IO ()
[X] putStr :: String -> IO ()
[X] putStrLn :: String -> IO ()
[X] print :: Show a => a -> IO ()
[X] getChar :: IO Char
[X] getLine :: IO String
[ ] getContents :: IO String
[ ] interact :: (String -> String) -> IO ()
[ ] type FilePath = String
[ ] readFile :: FilePath -> IO String
[ ] writeFile :: FilePath -> String -> IO ()
[ ] appendFile :: FilePath -> String -> IO ()
[ ] readIO :: Read a => String -> IO a
[X] readLn :: Read a => IO a
[ ] type IOError = IOException
[ ] ioError :: IOError -> IO a
[ ] userError :: String -> IOError
