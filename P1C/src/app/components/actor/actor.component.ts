import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { BruimdbServiceService } from '../../services/bruimdb-service.service';

@Component({
  selector: 'app-actor',
  templateUrl: './actor.component.html',
  styleUrls: ['./actor.component.css']
})
export class ActorComponent implements OnInit {

  constructor(
    private _bruimdbService: BruimdbServiceService,
    private _route: ActivatedRoute
  ) { }

  ngOnInit() {
    this._route.params
      .map(params => params['id'])
      .subscribe(id => {
      });
  }

}
