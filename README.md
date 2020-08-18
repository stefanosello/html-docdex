# html-docdex
A command-line utility to index html documents from url or local storage

[![GitHub license](https://img.shields.io/github/license/Naereen/StrapDown.js.svg)](https://github.com/Mr5he11/html-docdex/blob/master/LICENSE)


### Preface
This cli `c++` utility was created for a university exam. Its purpose is to let the users index html documents, in order to find them by searching through a keyword.
For example: a user finds interesting what is written in a webpacge (`www.pandasaregreat.com`). Then it indexes this page with the present tool.
When he/she wants to find the documents related to, for example, *pandas*, he/she can use this tool to retrieve the results. It's like a search engine based on text.

### Interface
```{bash}
$ docdex [-i <htmlpage>][-s <keyword>][-h]
```

**You want to index `www.pandasaregreat.com`?**

`docdex -i www.pandasaregreat.com`

**You want to search for pandas results?**

`docdex -s pandas`

### Details
Optionally, it is possible to change the default configurations.
Configurations tell the tool information about the weight certain html tags have (by default, for example, words wrapped in `h1` tags will be more important in a research),
the vocabulary used for stemming and other few things. A complete doc page will be available soon.
