-- phpMyAdmin SQL Dump
-- version 5.0.4
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Czas generowania: 25 Mar 2021, 18:23
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
-- Indeksy dla tabeli `orders`
--
ALTER TABLE `orders`
  ADD PRIMARY KEY (`Order_ID`);

--
-- AUTO_INCREMENT dla zrzuconych tabel
--

--
-- AUTO_INCREMENT dla tabeli `orders`
--
ALTER TABLE `orders`
  MODIFY `Order_ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
