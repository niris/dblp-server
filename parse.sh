#!/bin/bash

./xml2csv/xml2csv dblp.xml>../dblp.tsv
#mv ...
./csv2sql.pl<dblp.tsv


#import .sql
LOAD DATA LOCAL INFILE '/Users/sirin/Documents/WORKS/Doctorats/Codes/DBLP/xml/csv2sql/usr.tsv' INTO TABLE `usr` FIELDS TERMINATED BY '\t' LINES TERMINATED BY '\n';


SET GLOBAL max_heap_table_size = 1024 * 1024 * 1024 * 2;
SELECT @@max_heap_table_size

LOAD DATA LOCAL INFILE '/Users/sirin/Documents/WORKS/Doctorats/Codes/DBLP/Extraction_donnees/csv2sql/rel.tsv' INTO TABLE `rel` FIELDS TERMINATED BY '\t' LINES TERMINATED BY '\n';


#SELECT imbriqué .sql

select f_amis_k2.* from f_amis_k2 INNER JOIN
(
Select app_user, uid
From f_score_raffinee
) score
ON
score.app_user = f_amis_k2.uid and score.uid = f_amis_k2.friend_id
order by f_amis_k2.uid



#select
select * from f_ego_k2 e where (e.friend1 in (select s.uid from f_score_raffinee s where s.app_user = e.uid)) and  (e.friend2 in (select s.uid from f_score_raffinee s where s.app_user = e.uid))

#insert
INSERT INTO `f_ego_k2_pl` (select * from f_ego_k2 e where (e.friend1 in (select s.uid from f_score_raffinee s where s.app_user = e.uid)) and  (e.friend2 in (select s.uid from f_score_raffinee s where s.app_user = e.uid)))

#select
select * from f_publis_k2 p where p.uid in (select uid from f_score_raffinee)

INSERT INTO f_publis_k2_pl (select * from f_publis_k2 p where p.uid in (select uid from f_score_raffinee))


#Fusionner fichier
cat fichier_1.txt fichier_2.txt > textes_regroupes.txt


//update concate multiple rows
update f_interets f set f.interet = (select group_concat(tag) as tags from f_publis p where p.uid = f.uid) 

// update concate on row
update f_interets set interet = isnull(interet, '') + (blablabla)  // à vérifier si ça marche


collection_development,collection_development,coll...


SELECT `uid`,`alpha`,
avg(`cos_ego`),avg(`cos_all`),avg(`cos_indiv`),
avg(`prec_ego`),avg(`prec_all`),avg(`prec_indiv`),
avg(`rapp_ego`),avg(`rapp_all`),avg(`rapp_indiv`) 
FROM `f_analyses` 
GROUP BY  `alpha`



SELECT `alpha`,
avg(`prec_ego`),avg(`prec_indiv`),
avg(`rapp_ego`),avg(`rapp_indiv`)
FROM `f_analyses`
GROUP BY  `alpha`


SELECT `alpha`,
avg(`prec_ego`),avg(`prec_ego_k2_pl`),
avg(`rapp_ego`),avg(`rapp_ego_k2_pl`)
FROM `f_analyses_tmps`
GROUP BY  `alpha`

SELECT `alpha`,
avg(`prec_ego`),avg(`prec_ego_ts_adm`),avg(`prec_ego_ts_tms`),avg(`prec_ego_info`),
avg(`rapp_ego`),avg(`rapp_ego_ts_adm`),avg(`rapp_ego_ts_tms`),avg(`rapp_ego_info`),
FROM f_analyses
GROUP BY  `alpha`


SELECT `alpha`,
avg(`prec_ego`),avg(`prec_ego_ts_adm`),avg(`prec_ego_ts_tms`),avg(`prec_ego_ts_info`),avg(`prec_ego_ts_force`),
avg(`rapp_ego`),avg(`rapp_ego_ts_adm`),avg(`rapp_ego_ts_tms`),avg(`rapp_ego_ts_info`),avg(`rapp_ego_ts_force`)
FROM f_analyses
GROUP BY  `alpha`


SELECT `alpha`,
avg(`prec_ego`),avg(`prec_ego_ts_adm`),avg(`prec_ego_ts_info`),
avg(`rapp_ego`),avg(`rapp_ego_ts_adm`),avg(`rapp_ego_ts_info`)
FROM f_analyses
GROUP BY  `alpha`


SELECT `alpha`,
avg(`prec_ego`),avg(`prec_ego_ts_adm`),avg(`prec_ego_ts_force`),
avg(`rapp_ego`),avg(`rapp_ego_ts_adm`),avg(`rapp_ego_ts_force`)
FROM f_analyses
GROUP BY  `alpha`

SELECT `alpha`,
avg(`prec_ego`),avg(`prec_ego_ts_adm`),avg(`prec_ego_ts_tms`),avg(`prec_ego_ts_info`),
avg(`rapp_ego`),avg(`rapp_ego_ts_adm`),avg(`rapp_ego_ts_tms`),avg(`rapp_ego_ts_info`)
FROM f_analyses
GROUP BY  `alpha`



SELECT `alpha`,
avg(`prec_ego`),avg(`prec_ego_ts_adm`),avg(`prec_ego_ts_tms`),avg(`prec_ego_ts_info`),
avg(`rapp_ego`),avg(`rapp_ego_ts_adm`),avg(`rapp_ego_ts_tms`),avg(`rapp_ego_ts_info`)
FROM f_analyses_force
where uid in (select uid from f_interets_all where nbcom > 100 AND nbcom < 250) 
GROUP BY  `alpha`


SELECT `alpha`,
avg(`prec_ego`),avg(`prec_ego_ts_adm`),avg(`prec_ego_ts_tms`),avg(`prec_ego_ts_info`),
avg(`rapp_ego`),avg(`rapp_ego_ts_adm`),avg(`rapp_ego_ts_tms`),avg(`rapp_ego_ts_info`)
FROM f_analyses
where uid in (select uid from f_interets_all where nbcom > 80 AND nbcom < 500) 
and top = 10
GROUP BY  `alpha`

//marche 0.01

SELECT `alpha`,
avg(`prec_ego`),avg(`prec_ego_ts_adm`),avg(`prec_ego_ts_tms`),avg(`prec_ego_ts_info`),avg(`prec_indiv`),avg(`prec_indiv_ts`),
avg(`rapp_ego`),avg(`rapp_ego_ts_adm`),avg(`rapp_ego_ts_tms`),avg(`rapp_ego_ts_info`),avg(`rapp_indiv`),avg(`rapp_indiv_ts`)
FROM f_analyses
where uid in (select uid from f_interets_all where nbcom > 80 AND nbcom < 500) 
and top = 15
GROUP BY  `alpha`


SELECT  concat(year,' ', month, ' ', day),count(*), FROM `f_publis`
where titre = 'design'
group by  year, month
order by year,month,day 


SELECT  concat(year,' ', month),count(*) FROM `f_publis`
where titre = 'design'
group by  year, month
order by year,month,day


SELECT  titre, concat(year,' ', month),count(*) FROM `f_publis`
where titre in (select titre from f_tag_bookMarks
group by titre
order by count(*) 
limit 10 )
group by  year, month
order by year,month,day  


SELECT  titre, concat(year,' ', month),count(*) FROM `f_tag_bookMarks`
where titre in ('design','tools','tool','video','videos','webdesign','web','inspiration','art','arts','google')
group by titre,year, month
order by titre,year,month 



SELECT  concat(year,' ', month),count(uid) FROM `f_tag_bookMarks`
where titre in ('google')
group by year, month
ORDER BY year, month ASC
limit 500



SELECT `alpha`,
avg(`prec_ego`),avg(`prec_ego_ts_adm`),avg(`prec_ego_ts_tms`),avg(`prec_ego_ts_info`),avg(`prec_indiv`),avg(`prec_indiv_ts`),
avg(`rapp_ego`),avg(`rapp_ego_ts_adm`),avg(`rapp_ego_ts_info`),avg(`rapp_indiv`),avg(`rapp_indiv_ts`)
FROM f_analyses_85
where uid in (select uid from f_interets_all where nbcom > 80 AND nbcom < 500)
and top = 10
GROUP BY  `alpha`
