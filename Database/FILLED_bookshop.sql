-- phpMyAdmin SQL Dump
-- version 5.0.4
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Czas generowania: 30 Mar 2021, 15:30
-- Wersja serwera: 10.4.17-MariaDB
-- Wersja PHP: 8.0.0

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Baza danych: `bookshop`
--

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `access`
--

CREATE TABLE `access` (
  `Customer_ID` int(11) NOT NULL,
  `Login` text NOT NULL,
  `Password` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Zrzut danych tabeli `access`
--

INSERT INTO `access` (`Customer_ID`, `Login`, `Password`) VALUES
(1, 'schopenhauer', 'db5904a71d795025150aa1ef2fb20c14c099fde2cd0bce2a57f61c64cd9cd295'),
(2, 'aristotle', '23115eaaeebdb4a4afb9b4bb5d45931687500e6b8f1c8c701a02c3844e0051ee'),
(3, 'ibish', 'ec58a5ef0248761fe28f8feeb239e465e3973cdc60de3c76a9af2a47668b5674'),
(4, 'mithrandir', '762c06374d97f6fddc468069fbf6c27dd0d8b30ccb26c36458a9764c8bdb70a5'),
(5, 'nietzsche', '910b61b1d3d368f4aa203433db03f38f56c461fca8bda6f265aa21d38c22b607');

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `books`
--

CREATE TABLE `books` (
  `Book_ID` int(11) NOT NULL,
  `Author_name` text NOT NULL,
  `Author_surname` text NOT NULL,
  `Title` text NOT NULL,
  `Price` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Zrzut danych tabeli `books`
--

INSERT INTO `books` (`Book_ID`, `Author_name`, `Author_surname`, `Title`, `Price`) VALUES
(1, 'Artur', 'Schopenhauer', 'Erystka', 12),
(2, 'Jerzy', 'Grebosz', 'Opus Magnum', 100),
(3, 'Stanislaw', 'Lem', 'Solaris', 23),
(4, 'Mikhail', 'Bulghakov', 'Mistrz i Malgorzata', 34.5);

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `customers`
--

CREATE TABLE `customers` (
  `Customer_ID` int(11) NOT NULL,
  `Name` text NOT NULL,
  `Surname` text NOT NULL,
  `Email` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Zrzut danych tabeli `customers`
--

INSERT INTO `customers` (`Customer_ID`, `Name`, `Surname`, `Email`) VALUES
(1, 'John', 'Doe', 'j.doe@mail.com'),
(2, 'Jan', 'Kowalski', 'jan.k@mail.com'),
(3, 'Krzysztof', 'Ibish', 'k.ibisz@mail.com'),
(4, 'Ted', 'Kax', ''),
(5, 'Fryderyk', 'Chopin', 'f.ch@mail.com');

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `orders`
--

CREATE TABLE `orders` (
  `Order_ID` int(11) NOT NULL,
  `Customer_ID` int(11) NOT NULL,
  `Book_ID` int(11) NOT NULL,
  `Date` date NOT NULL,
  `Status` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Zrzut danych tabeli `orders`
--

INSERT INTO `orders` (`Order_ID`, `Customer_ID`, `Book_ID`, `Date`, `Status`) VALUES
(1, 4, 1, '2021-03-20', 'pending'),
(2, 4, 4, '2021-03-20', 'pending'),
(3, 3, 4, '2021-03-20', 'pending');

--
-- Indeksy dla zrzutów tabel
--

--
-- Indeksy dla tabeli `books`
--
ALTER TABLE `books`
  ADD PRIMARY KEY (`Book_ID`);

--
-- Indeksy dla tabeli `customers`
--
ALTER TABLE `customers`
  ADD PRIMARY KEY (`Customer_ID`);

--
-- Indeksy dla tabeli `orders`
--
ALTER TABLE `orders`
  ADD PRIMARY KEY (`Order_ID`);

--
-- AUTO_INCREMENT dla zrzuconych tabel
--

--
-- AUTO_INCREMENT dla tabeli `books`
--
ALTER TABLE `books`
  MODIFY `Book_ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;

--
-- AUTO_INCREMENT dla tabeli `customers`
--
ALTER TABLE `customers`
  MODIFY `Customer_ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- AUTO_INCREMENT dla tabeli `orders`
--
ALTER TABLE `orders`
  MODIFY `Order_ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
