library(dplyr)
library(knitr)

# Results for the first program's runs/time trials (10):

CrazyComputationV1.out <- data.frame(
  runNumber <- c(1:10),
  totalTime <- c(15.895665, 15.905715, 15.871590, 15.883040, 15.884898, 15.877612, 15.891883, 15.885114, 15.924263, 15.879044)
)  
  
CrazyComputationV1.out %>% summarize(totalTime.mean = mean(totalTime)) %>% kable()

# Results for the second program's runs/time trials (10):

CrazyComputationV2.out <- data.frame(
  runNumber <- c(1:10),
  totalTime <- c(15.881599, 15.872797, 15.878255, 15.855343, 15.877663, 15.886077, 15.885839, 15.877689, 15.920810, 15.922833),
  threadOneTime <- c(2.454162, 2.455153, 2.460714, 2.454984, 2.453552, 2.470633, 2.469262, 2.462372, 2.493782, 2.455486),
  threadTwoTime <- c(15.881590, 15.872730, 15.878194, 15.855279, 15.877596, 15.886016, 15.885777, 15.877626, 15.920747, 15.922775),
  loadImbalance <- c(13.427429, 13.417577, 13.417480, 13.400295, 13.424044, 13.415383, 13.416516, 13.415254, 13.426965, 13.467289)
) 

CrazyComputationV2.out %>% summarize(totalTime.mean = mean(totalTime),
                                           threadOneTime.mean = mean(threadOneTime),
                                           threadTwoTime.mean = mean(threadTwoTime),
                                           loadImbalance.mean = mean(loadImbalance)) %>% kable()

# Results for the third program's runs/time trials (10):

CrazyComputationV3.out <- data.frame(
  runNumber <- c(1:10),
  totalTime <- c(9.510011, 9.473649, 9.473957, 9.476412, 9.481967, 9.491735, 9.487484, 9.498229, 9.489242, 9.482246),
  threadOneTime <- c(9.510007, 9.473645, 8.984342, 9.476408, 9.481964, 9.491731, 9.487480, 9.498225, 9.489239, 8.984135),
  threadTwoTime <- c(9.005150, 8.965470, 9.473895, 8.990802, 8.975872, 8.981898, 8.977532, 8.994038, 8.973122, 9.482197),
  loadImbalance <- c(0.504857, 0.508175, 0.489553, 0.485606, 0.506092, 0.509834, 0.509948, 0.504187, 0.516117, 0.498062)
) 

CrazyComputationV3.out %>% summarize(totalTime.mean = mean(totalTime),
                                           threadOneTime.mean = mean(threadOneTime),
                                           threadTwoTime.mean = mean(threadTwoTime),
                                           loadImbalance.mean = mean(loadImbalance)) %>% kable()

# Results for the fourth program's runs/time trials (10):

CrazyComputationV4.out <- data.frame(
  runNumber <- c(1:10),
  totalTime <- c(9.452170, 9.492746, 9.477645, 9.465970, 9.491669, 9.484896, 9.470145, 9.485047, 9.470778, 9.469244),
  threadOneTime <- c(8.949385, 8.979706, 8.974267, 8.977941, 8.985423, 8.953821, 8.973516, 8.978712, 8.979374, 8.971126),
  threadTwoTime <- c(9.452099, 9.492680, 9.477577, 9.465905, 9.491603, 9.484829, 9.470083, 9.484978, 9.470710, 9.469214),
  loadImbalance <- c(0.502714, 0.512973, 0.503310, 0.487964, 0.506180, 0.531007, 0.496567, 0.506266, 0.491336, 0.498088)
) 

CrazyComputationV4.out %>% summarize(totalTime.mean = mean(totalTime),
                                           threadOneTime.mean = mean(threadOneTime),
                                           threadTwoTime.mean = mean(threadTwoTime),
                                           loadImbalance.mean = mean(loadImbalance)) %>% kable()