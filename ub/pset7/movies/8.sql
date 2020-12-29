--select people.name from people inner join stars on  stars.movie_id = 114709
select name from people inner join stars on stars.person_id = people.id
inner join movies on stars.movie_id = movies.id where movies.id = 114709