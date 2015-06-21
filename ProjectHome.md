# DBICP project #

## Abstract ##
_The DBICP Project studies a point based registration algorithm called the [“Dual Bootstrap Iterative Closest Point”](http://dbicp.googlecode.com/files/Dual%20Bootstrap%20ICP%20Algo.pdf)._

_This algorithm extends the classic Iterative Closest Point (ICP) algorithm, to overcome issues such as initialization sensitivity, few overlap, and unreliable matches. The innovations are made in the algorithm's structure, where the region used – the bootstrap region –, and the parametric transformation model selected are progressively “bootstrapped”, meaning enlarged._

_Finally, my source code is available on this website_

## Full report ##
[Download here](http://dbicp.googlecode.com/files/%5BFiot%5D%20The%20DBICP%20Project.pdf)

## Full presentation ##
[Download here](http://dbicp.googlecode.com/files/%5BFiot%5D%20The%20DBICP%20Project%20-%20Presentation.pdf)

## Demos ##
### Basic ICP ###
Here are a two examples, illustrating a stuck-in-local-min and successful optimizations using the Basic Iterative Closest Point algorithm. This shows the sensitivity to initialization.

| <a href='http://www.youtube.com/watch?feature=player_embedded&v=XzB5vkXr2mA' target='_blank'><img src='http://img.youtube.com/vi/XzB5vkXr2mA/0.jpg' width='425' height=344 /></a> | <a href='http://www.youtube.com/watch?feature=player_embedded&v=zbszjJnvNGk' target='_blank'><img src='http://img.youtube.com/vi/zbszjJnvNGk/0.jpg' width='425' height=344 /></a> |
|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

<a href='Hidden comment: 
|| <img src="http://dbicp.googlecode.com/files/Cost%20Graph%20-%20Basic%20ICP%20stuck%20in%20local%20min.jpg" width=425 height=425/>  || <img src="http://dbicp.googlecode.com/files/Cost%20Graph%20-%20Basic%20ICP%20successfull.jpg" width=425 height=425 /> ||
'></a>

### Model Boostrap ###
Here are a two examples:
  * in the first video, the parametric transformation is forced to be quadratic (no model bootstrap).
  * in the second video, the algorithm automatically selects the transformation model (model bootstrap activated). The results is clearly better, even if not perfect, but it illustrates well the idea.

| <a href='http://www.youtube.com/watch?feature=player_embedded&v=EReqsNxxzfE' target='_blank'><img src='http://img.youtube.com/vi/EReqsNxxzfE/0.jpg' width='425' height=344 /></a> | <a href='http://www.youtube.com/watch?feature=player_embedded&v=BsJ8QMGMZRg' target='_blank'><img src='http://img.youtube.com/vi/BsJ8QMGMZRg/0.jpg' width='425' height=344 /></a> |
|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|