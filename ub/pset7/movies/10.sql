select name from people inner join directors on directors.person_id = people.id
inner join movies on directors.movie_id = movies.id 
inner join ratings on ratings.movie_id=movies.id where ratings.rating = 9.0