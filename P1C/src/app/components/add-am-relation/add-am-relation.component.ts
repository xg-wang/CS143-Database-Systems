import { Component, OnInit } from '@angular/core';
import { FormData } from '../../models/form-data';
import { BruimdbServiceService } from '../../services/bruimdb-service.service';

@Component({
  selector: 'app-add-am-relation',
  templateUrl: './add-am-relation.component.html',
  styleUrls: ['./add-am-relation.component.css']
})
export class AddAmRelationComponent implements OnInit {
  relationForm = new FormData();

  constructor(private _bruimdbService: BruimdbServiceService) { }

  onSubmit() {
    this._bruimdbService.addRelation({
      'entity': 'actorMovie',
      'data': this.relationForm
    }).subscribe(res => {
      console.log(res);
    });
  }
  ngOnInit() {
  }

}
