select title from movies inner join stars on stars.movie_id = movies.id 
inner join people on people.id = stars.person_id where people.name IN ( "Johnny Depp", "Helena Bonham Carter")