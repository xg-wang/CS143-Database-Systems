import { Component, OnInit } from '@angular/core';
import { Movie } from '../../models/movie';
import { ActivatedRoute } from '@angular/router';
import { BruimdbServiceService } from '../../services/bruimdb-service.service';
import { ActorMovieRelation } from '../../models/actor-movie-relation';
import { Review } from '../../models/review';
import { Director } from '../../models/director';

@Component({
  selector: 'app-movie',
  templateUrl: './movie.component.html',
  styleUrls: ['./movie.component.css']
})
export class MovieComponent implements OnInit {
  movieInfo: Movie;
  director: Director;
  amRelations: ActorMovieRelation[];
  genres: string[];
  reviews: Review[];
  aveRating: number;
  reviewToAdd: Review = new Review();

  constructor(private _bruimdbService: BruimdbServiceService,
              private _route: ActivatedRoute) { }

  ngOnInit() {
    this.update();
  }
  update() {
    this._route.params
      .map(params => params['id'])
      .subscribe(id => {
        console.log(id);
        this._bruimdbService.searchMovieById(id)
          .subscribe(res => {
            this.movieInfo = res.movieInfo;
            this.director = res.director;
            this.amRelations = res.amRelations;
            this.genres = res.genres;
            this.reviews = res.reviews;
            this.aveRating = res.aveRating;
            console.log(this.reviews);
          });
      });
  }
  onClickAddReview() {
    this.reviewToAdd.title = this.movieInfo.title;
    this._bruimdbService.addEntity({
      'entity': 'review',
      'data': this.reviewToAdd
    }).subscribe(res => {
      console.log(res);
      this.update();
    });
  }
}
