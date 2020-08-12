# Word List

This directory contains a program, called vocab.c, which scans a large text and creates a count of all the words in the text.  Each word is bounded by space or a special character.  Only letters, numbers, and single quote are included in a single word.  The list of words generated with word counts is printed in alphabetic order, and may be sorted in numeric order with an additional step.

All words in the word list are converted to upper case, to eliminate problems of a capital letter at the beginning of a sentence or in a proper name.

In UTF-8 text, the single quote is not 0x27.  Therefore a single quote in a UTF-8 text does not show up in this program's output.  This problem has arisen in Gutenberg UTF-8 plain text books.  On the other hand, Gutenberg ASCII text uses a single quote as an embedded double quote.

Examples of running vocab:

cat moby_dick.txt | vocab | sort

This example prints the list of words in Moby Dick in word count order.
