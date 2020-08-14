# NOTES

### Perfect overlap tests for quant = 2000:

- state - commit : *bugfixes and Perfect overlap tests*

- margin for overlap to be meaningful :

    NOTE : assuming variance as in binominal distribution
    
        var = n * p * (1 - p)
        n = no of bits
        p = probability of 1 = 1/2
        var = n * 1/4
        3 sigma rule
        var * 3 = 3 * n / 4
        E(x) = n * p = n/2
        treshold = n/2 - 3n/4 = - n/4
        
    hence minimum is 0 threshold is set to be is 1/5 of n, where n is length of overlap

- min overlap 80 *(or higher not exceeding actual overlaps)* no errors with combinations:
    - len 150 shift 50 
    - len 200 shift 50 
    - len 250 shift 50 
    - len 300 shift 50 
    - len 200 shift 100 
    - len 250 shift 100 
    - len 300 shift 100 
    - len 350 shift 100 
    - len 400 shift 100
    - len 450 shift 100 

- for min overlap = 40
    - errors with len 100 shift 50
    
### Normal test and quant = 500, 1000, 2000

- state - commit : *Notes update*

- because of false alignments : -> threshold is set to be is 1/10

- for maps quanted with 500, 1000 errors happens at ends and sometimes in the middle
    
- Maps with quant 500 are very likely to be aligned incorrectly
    - Seems like maps quanted with lower values are more prone to false alignments *[mostly 0 -> mostly 0 from XOR operation]*
    
- for normally distributed maps - errors *[false alignments]*
    - typically maps are in line with local misplacements ??
    
### bad_alloc -- ecoli_1000quant_5cov_valid.binmap

- state - commit : *list protection in MapsContainer*

- ecoli_1000quant_5cov_valid.binmap : 
    - std::bad_alloc when writing output [draft map]
    - ecoli_1000quant_5cov_valid.binmap.details == no cycles
    - no draft map == working
    
- ecoli_1000quant_35cov_valid.binmap
    - std::bad_alloc
    - munmap_chunk(): invalid pointer *or* std::bad_alloc [when no draft_map out]

    

    
